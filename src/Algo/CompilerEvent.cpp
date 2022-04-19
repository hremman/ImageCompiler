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
        "Подготовка генераторов случайности",
        "Генерация палитр",
        "Проверка и кэширование изображений",
        "Кэширование инфрормации о цветах",
        "Подготовка заданий",
        "Компиляция изображений",
        "Сохранение полученных изображений"
    };
    return stages;
}

const QString & CCompiler::Event::desc(Stage stage)
{
    return stages()[static_cast<size_t>(stage)];
}


