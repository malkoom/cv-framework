#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string IMAGE = "../data/dog.jpg";
const std::string WINDOW = "P2 filters";
const std::vector<std::string> NOISE_NAMES = {"none", "gaussian", "salt and pepper"};
const std::vector<std::string> FILTER_NAMES = {"none", "box", "gauss", "median", "bilateral"};

/**
 * Añade a img el ruido elegido en el trackbar noise (ver NOISE_NAMES).
 *
 * - 0: sin ruido, devuelve img sin cambios.
 * - 1: ruido gaussiano aditivo de media 0 y desviación típica = level.
 * - 2: sal y pimienta en el % = level de los píxeles, elegidos al azar: la mitad pasan a negro (0) y la
 *   otra mitad a blanco (255).
 *
 * Los números aleatorios tienen que salir del generador de OpenCV, el que fija setRNGSeed.
 *
 * @param img        imagen de entrada, en escala de grises (CV_8U)
 * @param noiseType  tipo de ruido: 0, 1 o 2
 * @param level      gaussiano: desviación típica, en niveles de gris / sal y pimienta: % de píxeles
 * @return la imagen con ruido, en CV_8U
 */
cv::Mat addNoise(const cv::Mat &img, int noiseType, int level)
{
    //// TODO: CODE HERE
    cv::Mat result = img.clone();

    // Pistas
    // cv::randu = /2
    // cv:.randn = cb::add
    if (noiseType == 1)
    {
        cv::Mat noise(img.size(), CV_16S);
        cv::randn(noise, 0, level);
        cv::add(img, noise, result, cv::noArray(), CV_8U);
    }
    else if (noiseType == 2)
    {
        double amount = level / 100.0;
        cv::Mat u(img.size(), CV_32F);
        cv::randu(u, 0, 1);
        result.setTo(0, u < amount / 2);
        result.setTo(255, u > 1 - amount / 2);
    }

    //// END TODO
    return result;
}

/**
 * Aplica a img el filtro elegido en el trackbar filter (ver FILTER_NAMES).
 *
 * - 0: ninguno, devuelve img sin cambios.
 * - 1: box: cada píxel pasa a ser la media de su ventana de ksize x ksize.
 * - 2: gauss: filtro gaussiano con una ventana de ksize x ksize.
 * - 3: mediana: cada píxel pasa a ser la mediana de su ventana de ksize x ksize.
 * - 4: bilateral: suaviza respetando los bordes, con sigmaColor y sigmaSpace.
 *
 * Gauss y mediana solo admiten un ksize impar: con un ksize par no se aplican y devuelve img sin cambios.
 *
 * @param img         imagen de entrada
 * @param filterType  tipo de filtro: 0, 1, 2, 3 o 4
 * @param ksize       tamaño de la ventana
 * @param sigmaColor  bilateral: cuánto pueden diferir en intensidad los vecinos que se promedian
 * @param sigmaSpace  bilateral: cuánto pesan los vecinos según su distancia
 * @return la imagen filtrada
 */
cv::Mat applyFilter(const cv::Mat &img, int filterType, int ksize, int sigmaColor, int sigmaSpace)
{
    //// TODO: CODE HERE
    cv::Mat result = img.clone();
    switch (filterType)
    {
    case 0:
        break;

    case 1:
        cv::blur(img, result, cv::Size(ksize, ksize));
        break;

    case 2:
        cv::GaussianBlur(img, result, cv::Size(ksize, ksize), 0);
        break;

    case 3:
        cv::medianBlur(img, result, ksize);
        break;

    case 4:
        cv::bilateralFilter(img, result, ksize, sigmaColor, sigmaSpace);
        break;

    default:
        break;
    }
    //// END TODO
    return result;
}

// OpenCV la llama cada vez que se mueve el trackbar noise
void noiseChanged(int value, void *)
{
    std::cout << "Noise changed to " << NOISE_NAMES[value] << std::endl;
}

// OpenCV la llama cada vez que se mueve el trackbar filter
void filterChanged(int value, void *)
{
    std::cout << "Filter changed to " << FILTER_NAMES[value] << std::endl;
}

// OpenCV la llama cada vez que se mueve el trackbar k
void kChanged(int value, void *)
{
    if (value % 2 == 0)
    {
        std::cout << "k = " << value << " (even: gauss and median are not applied)" << std::endl;
    }
    else
    {
        std::cout << "k = " << value << std::endl;
    }
}

int main()
{
    cv::Mat img = cv::imread(IMAGE, cv::IMREAD_GRAYSCALE);
    if (img.empty())
    {
        std::cerr << "Error: could not read the image" << std::endl;
        return 1;
    }
    cv::pyrDown(img, img); // reducir a la mitad

    //// Trackbars
    cv::namedWindow(WINDOW);
    cv::createTrackbar("noise", WINDOW, nullptr, 2, noiseChanged);   // ver NOISE_NAMES
    cv::createTrackbar("level", WINDOW, nullptr, 100);               // gaussiano: desviación típica / sal y pimienta: % de píxeles
    cv::createTrackbar("filter", WINDOW, nullptr, 4, filterChanged); // ver FILTER_NAMES
    cv::createTrackbar("k", WINDOW, nullptr, 15, kChanged);          // tamaño del kernel (ventana de k x k píxeles)
    cv::createTrackbar("sigmaColor", WINDOW, nullptr, 200);          // solo bilateral
    cv::createTrackbar("sigmaSpace", WINDOW, nullptr, 50);           // solo bilateral
    // Default positions for the trackbars
    cv::setTrackbarPos("noise", WINDOW, 1);
    cv::setTrackbarPos("level", WINDOW, 20);
    cv::setTrackbarPos("filter", WINDOW, 1);
    cv::setTrackbarPos("k", WINDOW, 5);
    cv::setTrackbarPos("sigmaColor", WINDOW, 75);
    cv::setTrackbarPos("sigmaSpace", WINDOW, 5);
    // Minimum values for the trackbars
    cv::setTrackbarMin("k", WINDOW, 1);
    cv::setTrackbarMin("sigmaColor", WINDOW, 1);
    cv::setTrackbarMin("sigmaSpace", WINDOW, 1);

    while (true)
    {
        int noiseType = cv::getTrackbarPos("noise", WINDOW);
        int level = cv::getTrackbarPos("level", WINDOW);
        int filterType = cv::getTrackbarPos("filter", WINDOW);
        int ksize = cv::getTrackbarPos("k", WINDOW);
        int sigmaColor = cv::getTrackbarPos("sigmaColor", WINDOW);
        int sigmaSpace = cv::getTrackbarPos("sigmaSpace", WINDOW);

        cv::setRNGSeed(0);
        cv::Mat noisy = addNoise(img, noiseType, level);
        cv::Mat filtered = applyFilter(noisy, filterType, ksize, sigmaColor, sigmaSpace);

        // Gauss y mediana solo admiten un tamaño impar: avisar cuando no se aplican
        std::string warning = "";
        if ((filterType == 2 || filterType == 3) && ksize % 2 == 0)
        {
            warning = "   |   WARNING: even k, filter not applied";
        }

        cv::Mat view;
        cv::hconcat(noisy, filtered, view);
        cv::imshow(WINDOW, view);
        std::ostringstream title;
        title << std::fixed << std::setprecision(1)
              << "noisy: " << cv::PSNR(img, noisy) << " dB   |   filtered: " << cv::PSNR(img, filtered) << " dB"
              << warning;
        cv::setWindowTitle(WINDOW, title.str());
        if (cv::waitKey(30) == 27)
        {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
