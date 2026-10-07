#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
    // TODO: Crear y mostrar imagenes

    cv::Mat image_gray = cv::Mat(100, 300, CV_8UC1, 120);                   // 1 canal, valor 120
    cv::Mat image_blue = cv::Mat(100, 200, CV_8UC3, cv::Vec3b(255, 0, 0));  // 3 canales, BGR
    cv::Mat image_white = cv::Mat(100, 200, CV_8UC3, cv::Scalar::all(255)); // 3 canales, BGR, todos a 255

    std::cout << "Size: " << image_white.size() << std::endl; // [ancho x alto], al reves que shape
    std::cout << "Rows: " << image_white.rows << std::endl;
    std::cout << "Columns: " << image_white.cols << std::endl;
    std::cout << "Channels: " << image_white.channels() << std::endl;
    std::cout << "Depth: " << image_white.depth() << std::endl;
    cv::imshow("image_gray", image_gray);
    cv::imshow("image_blue", image_blue);
    cv::imshow("image_white", image_white);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Leer de fichero

    cv::Mat knights = cv::imread("../data/knights.png");
    cv::imshow("image_normal", knights);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Convertir a grayscale

    cv::Mat knights_gray;
    cv::cvtColor(knights, knights_gray, cv::COLOR_BGR2GRAY);
    cv::imshow("knights_gray", knights_gray);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Convertir a blanco y negro

    cv::Mat knights_bw;
    double value = cv::threshold(knights_gray, knights_bw, 100, 255, cv::THRESH_BINARY);
    cv::imshow("b&w", knights_bw);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Negativo
    cv::Mat knights_negative = cv::Scalar::all(255) - knights; // all(255): los 3 canales
    cv::imshow("knights_negative", knights_negative);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Multiplicar por escalar
    cv::Mat knights_darker;
    cv::multiply(knights, 0.5, knights_darker);
    cv::imshow("knights_darker", knights_darker);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Sumar y combinar
    cv::Mat chess = cv::imread("../data/chessboard.png");
    if (chess.empty())
    {
        std::cerr << "Error: no se ha podido leer la imagen" << std::endl;
        return 1;
    }
    cv::Mat fusion;
    cv::addWeighted(knights, 0.5, chess, 0.5, 0, fusion); // 0.5*a + 0.5*b + 0
    cv::imshow("fusion", fusion);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Lectura/escritura de pixeles
    cv::Vec3b px1 = knights.at<cv::Vec3b>(20, 20);                                                                     // (fila, columna) -> [B G R]
    std::cout << "Pixel (20, 20): [" << (int)px1[0] << ", " << (int)px1[1] << ", " << (int)px1[2] << "]" << std::endl; // cast a int y orden BGR
    for (int r = 20; r < 60; r++)
    {
        for (int c = 30; c < 90; c++)
        {
            knights.at<cv::Vec3b>(r, c) = cv::Vec3b(0, 0, 255); // bloque rojo
        }
    }
    cv::imshow("knights_red_block", knights);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Copia y clonado
    cv::Mat knights_ref = knights;           // NO copia: misma memoria
    knights_ref.setTo(cv::Vec3b(0, 255, 0)); // setTo -> set all pixels to a value
    cv::imshow("knights_ref", knights_ref);
    cv::imshow("knights", knights);
    cv::waitKey(0);

    knights = cv::imread("../data/knights.png"); // recuperamos la original
    cv::Mat knights_copy = knights.clone();      // copia real
    knights_copy.setTo(cv::Vec3b(0, 255, 0));
    cv::imshow("knights clone", knights_copy);
    cv::imshow("knights", knights);
    cv::waitKey(0);

    cv::destroyAllWindows();

    // TODO: Crop
    cv::Rect rectangle(450, 420, 70, 70);      // (x, y, ancho, alto)
    cv::Mat face = knights(rectangle).clone(); // clone() para que sea copia real
    cv::imshow("knights", knights);
    cv::imshow("face", face);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Resize
    cv::Mat face_big;
    cv::resize(face, face_big, cv::Size(420, 420)); // por defecto INTER_LINEAR
    cv::imshow("face_big_linear", face_big);

    cv::Mat face_big_nearest;
    cv::resize(face, face_big_nearest, cv::Size(420, 420), 0, 0, cv::INTER_NEAREST);
    cv::imshow("face_big_nearest", face_big_nearest);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: Canales
    std::vector<cv::Mat> bgr;
    cv::split(knights, bgr); // bgr[0]=B, bgr[1]=G, bgr[2]=R
    cv::imshow("blue", bgr[0]);
    cv::imshow("green", bgr[1]);
    cv::imshow("red", bgr[2]);
    cv::waitKey(0);
    cv::destroyAllWindows();

    // TODO: HSV
    cv::Mat hsv;
    cv::cvtColor(knights, hsv, cv::COLOR_BGR2HSV); // H [0,180), S [0,255], V [0,255]
    std::vector<cv::Mat> planes;
    cv::split(hsv, planes); // planes[0]=H, [1]=S, [2]=V

    cv::multiply(planes[1], 2.0, planes[1]); // saturacion x2 (satura en 255)
    cv::Mat hsv_sat, knights_sat;
    cv::merge(planes, hsv_sat);
    cv::cvtColor(hsv_sat, knights_sat, cv::COLOR_HSV2BGR);
    cv::imshow("knights", knights);
    cv::imshow("saturation x2", knights_sat);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
