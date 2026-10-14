#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

const std::string IMAGE = "../data/dog.jpg";

/**
 * Reduce img a la mitad steps veces seguidas.
 *
 * @param img    imagen de entrada, en escala de grises
 * @param steps  número de veces que se reduce a la mitad
 * @return la imagen reducida: 2^steps veces más pequeña en ancho y en alto
 */
cv::Mat downscalePyr(const cv::Mat& img, int steps) {
    //// TODO: CODE HER
    cv::Mat downScaled = img.clone();
    
    for (size_t i = 0; i < steps; i++)
    {
        cv::pyrDown(downScaled, downScaled);
    }

    return downScaled;
}

/**
 * Amplía img al doble steps veces seguidas.
 *
 * @param img    imagen de entrada, en escala de grises
 * @param steps  número de veces que se amplía al doble
 * @return la imagen ampliada: 2^steps veces más grande en ancho y en alto
 */
cv::Mat upscalePyr(const cv::Mat& img, int steps) {
    //// TODO: CODE HERE
    cv::Mat upScaled = img.clone();
    
    for (size_t i = 0; i < steps; i++)
    {
        cv::pyrUp(upScaled, upScaled);
    }

    return upScaled;
}

/**
 * Redimensiona img al tamaño size.
 *
 * @param img   imagen que se redimensiona
 * @param size  tamaño de salida (ancho, alto)
 * @return img redimensionada a size
 */
cv::Mat resizeNearest(const cv::Mat& img, cv::Size size) {
    //// TODO: CODE HERE
    cv::Mat resized;
    cv::resize(img, resized, size, 0, 0, cv::INTER_NEAREST);
    return resized;
}

/**
 * Recorta img a height filas y width columnas, quedándose con la esquina superior izquierda.
 *
 * @param img     imagen que se recorta; tiene que medir al menos height x width
 * @param height  número de filas de la salida
 * @param width   número de columnas de la salida
 * @return la esquina superior izquierda de img, de height filas y width columnas
 */
cv::Mat fixSize(const cv::Mat& img, int height, int width) {
    //// TODO: CODE HERE
    auto result = img(cv::Rect(0,0,width,height));
    return result;
}

/**
 * Diferencia absoluta entre a y b, multiplicada por alpha para que se vea.
 *
 * @param a, b   imágenes del mismo tamaño y tipo
 * @param alpha  factor por el que se multiplica la diferencia
 * @return la diferencia amplificada, en uint8
 */
cv::Mat difference(const cv::Mat& a, const cv::Mat& b, double alpha) {
    cv::Mat diff, amplified;
    cv::absdiff(a, b, diff);
    cv::multiply(diff, alpha, amplified);
    return amplified;
}

/**
 * Muestra cada imagen en su ventana y espera a que se pulse una tecla.
 *
 * @param windows  lista de pares {nombre de ventana, imagen}
 */
void show(const std::vector<std::pair<std::string, cv::Mat>>& windows) {
    for (const auto& [name, image] : windows) {
        cv::imshow(name, image);
    }
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main() {
    //// Cargar la imagen en escala de grises
    cv::Mat img = cv::imread(IMAGE, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Error: could not read the image" << std::endl;
        return 1;
    }

    //// 1. Reducir: pyrDown downSteps veces frente a un único resize al mismo tamaño
    int downSteps = 3;
    cv::Mat smallPyr = downscalePyr(img, downSteps);
    cv::Mat smallResize = resizeNearest(img, smallPyr.size());
    show({
        {"pyrDown x" + std::to_string(downSteps), smallPyr},
        {"resize", smallResize},
        {"difference x3", difference(smallPyr, smallResize, 3)},
    });

    //// 2. Ampliar x2: pyrUp frente a resize al doble de tamaño
    cv::Mat bigPyr = upscalePyr(img, 1);
    cv::Mat bigResize = resizeNearest(img, bigPyr.size());
    show({
        {"pyrUp", bigPyr},
        {"resize", bigResize},
        {"difference x5", difference(bigPyr, bigResize, 5)},   // x5: si no, se ve negra
    });

    //// 3. Ida y vuelta: lo que pyrDown tira, pyrUp no lo recupera
    int steps = 2;
    cv::Mat roundTrip = upscalePyr(downscalePyr(img, steps), steps);
    roundTrip = fixSize(roundTrip, img.rows, img.cols);   // la vuelta sale más grande que img
    show({
        {"original", img},
        {"round trip", roundTrip},
        {"difference x5", difference(img, roundTrip, 5)},
    });

    return 0;
}
