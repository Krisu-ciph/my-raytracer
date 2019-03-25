template <typename... Args>
inline std::string StringPrintf(const char *fmt, Args... args) {
    std::string ret;
    StringPrintfRecursive(&ret, fmt, args...);
    return ret;
}
