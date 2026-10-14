#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <string>
#include <utility>
#include <vector>

const std::string IMAGE = "../data/fotocarmen.jpg";

/**
 * Convierte img en una imagen con estilo cómic/dibujo animado.
 *
 * @param img  imagen de entrada, en color (BGR)
 * @return la imagen con el efecto, en BGR (CV_8UC3) y del mismo tamaño que img
 */
cv::Mat cartoonize(const cv::Mat &img)
{
    // -------------------------------------------------------------
    // 1. Detección y engrosado de bordes tipo tinta negra
    // -------------------------------------------------------------
    cv::Mat gray, blurredGray, edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    
    // Filtro de mediana para eliminar ruido fino y dejar solo contornos claros
    cv::medianBlur(gray, blurredGray, 7);

    // Umbralizado adaptativo para generar líneas negras sobre fondo blanco
    cv::adaptiveThreshold(blurredGray, edges, 255, 
                          cv::ADAPTIVE_THRESH_MEAN_C, 
                          cv::THRESH_BINARY, 9, 2);

    // -------------------------------------------------------------
    // 2. Simplificación de color (texturas planas)
    // -------------------------------------------------------------
    cv::Mat colorSmoothed = img.clone();
    
    // Varias pasadas de filtro bilateral eliminan el ruido/textura
    // manteniendo las fronteras de color intactas
    for (int i = 0; i < 4; ++i)
    {
        cv::Mat temp;
        cv::bilateralFilter(colorSmoothed, temp, 9, 75, 75);
        colorSmoothed = temp;
    }

    // -------------------------------------------------------------
    // 3. Fusión de bordes negros con el color simplificado
    // -------------------------------------------------------------
    // Convertir la máscara de bordes (1 canal) a 3 canales BGR
    cv::Mat edgesBgr;
    cv::cvtColor(edges, edgesBgr, cv::COLOR_GRAY2BGR);

    // Donde 'edges' sea 0 (borde) quedará negro; donde sea 255 se mantendrá el color
    cv::Mat result;
    cv::bitwise_and(colorSmoothed, edgesBgr, result);

    return result;
}

/**
 * Los efectos que ya trae OpenCV en el módulo photo, para compararlos con el propio.
 *
 * @param img  imagen de entrada, en color (BGR)
 * @return lista de pares {nombre de ventana, imagen} con cada efecto
 */
std::vector<std::pair<std::string, cv::Mat>> photoEffects(const cv::Mat &img)
{
    cv::Mat stylized, preserved, sketchGray, sketchColor, detailed;
    cv::stylization(img, stylized, 60, 0.45f);
    cv::edgePreservingFilter(img, preserved, cv::RECURS_FILTER, 60, 0.4f);
    cv::pencilSketch(img, sketchGray, sketchColor, 60, 0.07f, 0.02f);
    cv::detailEnhance(img, detailed, 10, 0.15f);

    return {
        {"stylization", stylized},
        {"edgePreservingFilter", preserved},
        {"pencilSketch (gray)", sketchGray},
        {"pencilSketch (colour)", sketchColor},
        {"detailEnhance", detailed},
    };
}

int main()
{
    // Cargar la imagen en color
    cv::Mat img = cv::imread(IMAGE);
    if (img.empty())
    {
        std::cerr << "Error: could not read the image" << std::endl;
        return 1;
    }

    // 1. Procesar con tu filtro personalizado
    cv::Mat cartoon = cartoonize(img);

    // Redimensionar para pantalla completa / visualización cómoda
    cv::Mat cartoonPreview;
    cv::resize(cartoon, cartoonPreview, cv::Size(540, 960), 0.5, 0.5, cv::INTER_AREA);
    cv::imshow("cartoon (propio)", cartoonPreview);

    // 2. Comparar con los efectos nativos de OpenCV
    // for (const auto &[name, effect] : photoEffects(img))
    // {
    //     cv::Mat preview;
    //     cv::resize(effect, preview, cv::Size(1080, 1090), 0, 0, cv::INTER_AREA);
    //     cv::imshow(name, preview); // Corregido: mostrar 'preview' en vez de 'effect'
    // }

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}