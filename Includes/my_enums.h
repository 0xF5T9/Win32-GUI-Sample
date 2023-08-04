#ifndef MY_ENUMS_H
#define MY_ENUMS_H

enum class MyTheme
{
    Light,
    Dark,
    Monokai,
};
enum class MyLogType
{
    Info,
    Warn,
    Error,
    Debug,
};
enum class MyWindowType
{
    Undefined,
    Unmanaged,
    Static,
    StandardButton,
    ImageButton,
    RadioButton,
    Edit,
    DDLCombobox,
    Scrollbar,
    Container
};
enum class MyEditboxType
{
    Singleline,
    SinglelinePassword,
    Multiline
};
enum class MyImageFormat
{
    Unknown,
    JPG,
    PNG,
    ICO
};

#endif // MY_ENUMS_H