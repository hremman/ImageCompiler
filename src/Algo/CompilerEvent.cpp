#include "CompilerEvent.hpp"

const QString & CCompiler::Event::desc(Type type)
{
    static QString types[] = {
        "Выполнено",
        "Предупреждение",
        "Ошибка",
        "Прогресс"
    };
    return types[static_cast<size_t>(type)];
}

const QString * CCompiler::Event::stages()
{
    static const QString stages[] = {
        "Подготовка контекста компиляции",
        "Проверка и кэширование изображений",
        "Подготовка генераторов случайности",
        "Генерация палитр",
        "Кэширование инфрормации о цветах",
        "Компиляция изображений",
        "Сохранение полученных изображений"
    };
    return stages;
}

const QString & CCompiler::Event::desc(Stage stage)
{
    return stages()[static_cast<size_t>(stage)];
}


