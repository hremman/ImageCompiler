#include <random>
#include <list>
#include <vector>
#include <map>
#include <QTemporaryFile>
#include "Processor.hpp"
#include "ImageProcessing.hpp"

CProcessor CProcessor::__M_instance = CProcessor();

CProcessor::CProcessor()
    : QObject(nullptr)
    , m_workers(0)
{

}

bool CProcessor::Build(const Data::CProject & proj)
{return __M_instance.build(proj);}

bool CProcessor::BuildOne(/*1 таска*/)
    {{return __M_instance.buildOne(/*1 таска*/);}}

bool CProcessor::build(const Data::CProject & proj) const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution color_gen(0,255);
    std::map<Data::CLayer *, std::bernoulli_distribution> layer_to_noise;
    std::map<Data::CLayer *, std::bernoulli_distribution> layer_to_use;

    std::map<QString, QImage*> file_to_image;
    std::map<QString, ImageCache> file_to_cache;

    std::map<QImage*, ImageCache> image_to_cache;
    std::map<Data::CLayer *, std::vector<Data::CColor>> layer_to_paletes;

    auto layers = proj.layers();

    //add_line("Генерация цветов");

    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        if ( (*it)->m_type == Data::CLayer::WorkType::ENUMERATION )
        {
            if ((*it)->m_colors.m_mode == Data::CColorSettings::Mode::GENERATION)
            {
                layer_to_paletes[*it] = std::vector<Data::CColor>();
                layer_to_paletes[*it].resize((*it)->m_colors.m_generations_number);
                for (size_t i = 0; i < layer_to_paletes[*it].size(); i++)
                    layer_to_paletes[*it][i] = Data::CColor(color_gen(gen),color_gen(gen),color_gen(gen) );
            }
        }
    }



    //add_line("Проверка вероятностей");

    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        if ( (*it)->m_noise_probability != 0.0 )
        {
            layer_to_noise[*it] = std::bernoulli_distribution((*it)->m_use_probability < 1.0 ? (*it)->m_use_probability : 1);
        }

        if ( (*it)->m_use_probability != 0.0 )
        {
            layer_to_use[*it] = std::bernoulli_distribution((*it)->m_use_probability < 1.0 ? (*it)->m_use_probability: 1 );
        }
    }

    //add_line("Проверка изображений");

    //Открываем файлы, дампим их во времянку и запоминаем сдвиг
    //Потом из неё вынем и будем юзать
    //QTemporaryFile



    //add_line("Построение кэшей");
    //add_line("Подготовка заданий");
    //add_line("Генерация изображений");


}
