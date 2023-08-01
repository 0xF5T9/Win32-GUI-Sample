##### Error check and logging snippets.
```C++
// Logging.
WriteLog(L"", L"", MyLogType::Info);
WriteLog(L"", L"", MyLogType::Warn);
WriteLog(L"", L"", MyLogType::Error);
WriteLog(L"", L"", MyLogType::Debug);
// Error check.
bool are_all_operation_success = false;
std::wstring error_message = L"";
while (!are_all_operation_success)
{
    // ...

    are_all_operation_success = true;
}

if (!are_all_operation_success)
{
    WriteLog(error_message, L"", MyLogType::Error);
    /// ...
}
// [STRUCT: \"\" | FUNC: \"\"]
// [CLASS: \"\" | FUNC: \"\"]
// [CLASS: \"\" | MESSAGE: \"\" | CALLBACK: \"\"]
// [NAMESPACE: \"\" | FUNC: \"\"]
// [MESSAGE: \"\" | CALLBACK: \"\"]
```