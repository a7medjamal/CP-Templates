#include <bits/stdc++.h>
using namespace std;

namespace StringUtilities {
    // Lyndon Words and Duval's Algorithm
    class LyndonWords {
    public:
        static vector<string> duvalFactorization(string s) {
            vector<string> factors;
            int n = s.length();
            int i = 0;
            
            while (i < n) {
                int j = i + 1, k = i;
                
                while (j < n && s[k] <= s[j]) {
                    if (s[k] < s[j]) {
                        k = i;
                    } else {
                        k++;
                    }
                    j++;
                }
                
                while (i <= k) {
                    factors.push_back(s.substr(i, j - k));
                    i += j - k;
                }
            }
            
            return factors;
        }
        
        static string minimalRotation(string s) {
            s += s;
            int n = s.length() / 2;
            int i = 0, ans = 0;
            
            while (i < n) {
                ans = i;
                int j = i + 1, k = i;
                
                while (j < i + n && s[k] <= s[j]) {
                    if (s[k] < s[j]) {
                        k = i;
                    } else {
                        k++;
                    }
                    j++;
                }
                
                while (i <= k) {
                    i += j - k;
                }
            }
            
            return s.substr(ans, n);
        }
        
        static bool isLyndonWord(string s) {
            for (int i = 1; i < s.length(); i++) {
                if (s < s.substr(i) + s.substr(0, i)) {
                    return false;
                }
            }
            return true;
        }
        
        static vector<string> generateLyndonWords(int n, string alphabet) {
            vector<string> result;
            
            function<void(string, int)> generate = [&](string current, int lastChar) {
                if (current.length() == n) {
                    if (isLyndonWord(current)) {
                        result.push_back(current);
                    }
                    return;
                }
                
                for (int i = lastChar; i < alphabet.length(); i++) {
                    generate(current + alphabet[i], i);
                }
            };
            
            generate("", 0);
            return result;
        }
    };
    
    // String transformations
    namespace Transformations {
        string reverse(string s) {
            reverse(s.begin(), s.end());
            return s;
        }
        
        string toLowerCase(string s) {
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }
        
        string toUpperCase(string s) {
            transform(s.begin(), s.end(), s.begin(), ::toupper);
            return s;
        }
        
        string removeSpaces(string s) {
            s.erase(remove(s.begin(), s.end(), ' '), s.end());
            return s;
        }
        
        string removeChar(string s, char c) {
            s.erase(remove(s.begin(), s.end(), c), s.end());
            return s;
        }
        
        vector<string> split(string s, char delimiter) {
            vector<string> result;
            stringstream ss(s);
            string token;
            
            while (getline(ss, token, delimiter)) {
                result.push_back(token);
            }
            
            return result;
        }
        
        string join(vector<string>& strings, string delimiter) {
            if (strings.empty()) return "";
            
            string result = strings[0];
            for (int i = 1; i < (int)strings.size(); i++) {
                result += delimiter + strings[i];
            }
            return result;
        }
        
        string trim(string s) {
            size_t start = s.find_first_not_of(" \t\n\r");
            if (start == string::npos) return "";
            
            size_t end = s.find_last_not_of(" \t\n\r");
            return s.substr(start, end - start + 1);
        }
        
        string replaceAll(string s, string from, string to) {
            size_t pos = 0;
            while ((pos = s.find(from, pos)) != string::npos) {
                s.replace(pos, from.length(), to);
                pos += to.length();
            }
            return s;
        }
        
        string padLeft(string s, int width, char padChar = ' ') {
            if (s.length() >= width) return s;
            return string(width - s.length(), padChar) + s;
        }
        
        string padRight(string s, int width, char padChar = ' ') {
            if (s.length() >= width) return s;
            return s + string(width - s.length(), padChar);
        }
    }
    
    // String comparison and similarity
    namespace Similarity {
        int levenshteinDistance(string s1, string s2) {
            int m = s1.length(), n = s2.length();
            vector<vector<int>> dp(m + 1, vector<int>(n + 1));
            
            for (int i = 0; i <= m; i++) dp[i][0] = i;
            for (int j = 0; j <= n; j++) dp[0][j] = j;
            
            for (int i = 1; i <= m; i++) {
                for (int j = 1; j <= n; j++) {
                    if (s1[i-1] == s2[j-1]) {
                        dp[i][j] = dp[i-1][j-1];
                    } else {
                        dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
                    }
                }
            }
            
            return dp[m][n];
        }
        
        int hammingDistance(string s1, string s2) {
            if (s1.length() != s2.length()) return -1;
            
            int distance = 0;
            for (int i = 0; i < s1.length(); i++) {
                if (s1[i] != s2[i]) distance++;
            }
            return distance;
        }
        
        double jaccardSimilarity(string s1, string s2) {
            set<char> set1(s1.begin(), s1.end());
            set<char> set2(s2.begin(), s2.end());
            
            set<char> intersection, unionSet;
            set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
                           inserter(intersection, intersection.begin()));
            set_union(set1.begin(), set1.end(), set2.begin(), set2.end(),
                     inserter(unionSet, unionSet.begin()));
            
            if (unionSet.empty()) return 1.0;
            return (double)intersection.size() / unionSet.size();
        }
        
        string longestCommonSubsequence(string s1, string s2) {
            int m = s1.length(), n = s2.length();
            vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
            
            for (int i = 1; i <= m; i++) {
                for (int j = 1; j <= n; j++) {
                    if (s1[i-1] == s2[j-1]) {
                        dp[i][j] = dp[i-1][j-1] + 1;
                    } else {
                        dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                    }
                }
            }
            
            // Reconstruct LCS
            string lcs = "";
            int i = m, j = n;
            while (i > 0 && j > 0) {
                if (s1[i-1] == s2[j-1]) {
                    lcs = s1[i-1] + lcs;
                    i--; j--;
                } else if (dp[i-1][j] > dp[i][j-1]) {
                    i--;
                } else {
                    j--;
                }
            }
            
            return lcs;
        }
        
        string longestCommonSubstring(string s1, string s2) {
            int m = s1.length(), n = s2.length();
            vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
            int maxLen = 0, endPos = 0;
            
            for (int i = 1; i <= m; i++) {
                for (int j = 1; j <= n; j++) {
                    if (s1[i-1] == s2[j-1]) {
                        dp[i][j] = dp[i-1][j-1] + 1;
                        if (dp[i][j] > maxLen) {
                            maxLen = dp[i][j];
                            endPos = i;
                        }
                    }
                }
            }
            
            return s1.substr(endPos - maxLen, maxLen);
        }
    }
    
    // String generation and validation
    namespace Generation {
        string generateRandomString(int length, string charset = "abcdefghijklmnopqrstuvwxyz") {
            string result = "";
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, charset.length() - 1);
            
            for (int i = 0; i < length; i++) {
                result += charset[dis(gen)];
            }
            
            return result;
        }
        
        vector<string> generatePermutations(string s) {
            vector<string> result;
            sort(s.begin(), s.end());
            
            do {
                result.push_back(s);
            } while (next_permutation(s.begin(), s.end()));
            
            return result;
        }
        
        vector<string> generateSubstrings(string s) {
            vector<string> result;
            int n = s.length();
            
            for (int i = 0; i < n; i++) {
                for (int j = i; j < n; j++) {
                    result.push_back(s.substr(i, j - i + 1));
                }
            }
            
            return result;
        }
        
        vector<string> generateSubsequences(string s) {
            vector<string> result;
            int n = s.length();
            
            for (int mask = 0; mask < (1 << n); mask++) {
                string subsequence = "";
                for (int i = 0; i < n; i++) {
                    if (mask & (1 << i)) {
                        subsequence += s[i];
                    }
                }
                result.push_back(subsequence);
            }
            
            return result;
        }
    }
    
    // String validation
    namespace Validation {
        bool isAlphabetic(string s) {
            return all_of(s.begin(), s.end(), ::isalpha);
        }
        
        bool isNumeric(string s) {
            return all_of(s.begin(), s.end(), ::isdigit);
        }
        
        bool isAlphanumeric(string s) {
            return all_of(s.begin(), s.end(), ::isalnum);
        }
        
        bool isValidEmail(string email) {
            regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
            return regex_match(email, emailRegex);
        }
        
        bool isValidURL(string url) {
            regex urlRegex(R"(https?://[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(/.*)?)");
            return regex_match(url, urlRegex);
        }
        
        bool isBalancedParentheses(string s) {
            stack<char> st;
            map<char, char> pairs = {{')', '('}, {']', '['}, {'}', '{'}};
            
            for (char c : s) {
                if (c == '(' || c == '[' || c == '{') {
                    st.push(c);
                } else if (c == ')' || c == ']' || c == '}') {
                    if (st.empty() || st.top() != pairs[c]) {
                        return false;
                    }
                    st.pop();
                }
            }
            
            return st.empty();
        }
    }
    
    // Advanced string operations
    namespace Advanced {
        vector<int> computeBorderArray(string s) {
            int n = s.length();
            vector<int> border(n, 0);
            
            for (int i = 1; i < n; i++) {
                int j = border[i-1];
                while (j > 0 && s[i] != s[j]) {
                    j = border[j-1];
                }
                if (s[i] == s[j]) j++;
                border[i] = j;
            }
            
            return border;
        }
        
        vector<int> getAllPeriods(string s) {
            vector<int> periods;
            vector<int> border = computeBorderArray(s);
            int n = s.length();
            
            int period = n - border[n-1];
            while (period <= n) {
                if (n % period == 0) {
                    periods.push_back(period);
                }
                if (border[n-1] == 0) break;
                period = n - border[border[n-1] - 1];
            }
            
            return periods;
        }
        
        int countDistinctSubstrings(string s) {
            set<string> substrings;
            int n = s.length();
            
            for (int i = 0; i < n; i++) {
                for (int j = i; j < n; j++) {
                    substrings.insert(s.substr(i, j - i + 1));
                }
            }
            
            return substrings.size();
        }
        
        string compress(string s) {
            if (s.empty()) return "";
            
            string result = "";
            int count = 1;
            char current = s[0];
            
            for (int i = 1; i < s.length(); i++) {
                if (s[i] == current) {
                    count++;
                } else {
                    result += current;
                    if (count > 1) {
                        result += to_string(count);
                    }
                    current = s[i];
                    count = 1;
                }
            }
            
            result += current;
            if (count > 1) {
                result += to_string(count);
            }
            
            return result.length() < s.length() ? result : s;
        }
        
        string decompress(string s) {
            string result = "";
            int i = 0;
            
            while (i < s.length()) {
                char c = s[i++];
                int count = 1;
                
                if (i < s.length() && isdigit(s[i])) {
                    count = 0;
                    while (i < s.length() && isdigit(s[i])) {
                        count = count * 10 + (s[i] - '0');
                        i++;
                    }
                }
                
                result += string(count, c);
            }
            
            return result;
        }
    }
}

// Example usage:
/*
int main() {
    using namespace StringUtilities;
    
    string text = "abcabc";
    
    // Lyndon words
    cout << "Minimal rotation: " << LyndonWords::minimalRotation(text) << endl;
    cout << "Is Lyndon word: " << LyndonWords::isLyndonWord(text) << endl;
    
    // Transformations
    cout << "Uppercase: " << Transformations::toUpperCase(text) << endl;
    cout << "Reversed: " << Transformations::reverse(text) << endl;
    
    // Similarity
    string text2 = "abcdef";
    cout << "Levenshtein distance: " << Similarity::levenshteinDistance(text, text2) << endl;
    cout << "LCS: " << Similarity::longestCommonSubsequence(text, text2) << endl;
    
    // Validation
    cout << "Is alphabetic: " << Validation::isAlphabetic(text) << endl;
    cout << "Balanced parentheses: " << Validation::isBalancedParentheses("(())") << endl;
    
    // Advanced
    cout << "Compressed: " << Advanced::compress("aaabbbccc") << endl;
    cout << "Distinct substrings: " << Advanced::countDistinctSubstrings(text) << endl;
    
    return 0;
}
*/