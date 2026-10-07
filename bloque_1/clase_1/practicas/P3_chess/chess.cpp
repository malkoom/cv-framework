#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <iostream>

cv::Mat createChessboard(int rows, int cols, int square_size)
{
    if (rows % square_size != 0 || cols % square_size != 0)
    {
        std::cerr << "Error: The chessboard size is not a multiple of the square size." << std::endl;
        exit(1);
    }

    cv::Mat img(rows, cols, CV_8UC3, cv::Vec3b(255, 255, 255));

    // TODO: recorrer la imagen de casilla en casilla y pintar de negro las que
    // correspondan, hasta formar el patron de tablero.
    for (int i = 0; i < img.rows; i += square_size)
    {
        for (int j = 0; j < img.cols; j += square_size)
        {
            if (((i / square_size) + (j / square_size)) % 2 == 0)
            {
                img(cv::Rect(j, i, square_size, square_size)).setTo(cv::Vec3b(0, 0, 0));
            }
        }
    }

    return img;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <rows> <cols> <square_size>" << std::endl;
        return 1;
    }
    int rows = std::stoi(argv[1]);
    int cols = std::stoi(argv[2]);
    int square_size = std::stoi(argv[3]);
    cv::Mat img = createChessboard(rows, cols, square_size);
    cv::imshow("Chessboard", img);
    cv::waitKey(0);
    cv::imwrite("chessboard.png", img);
    return 0;
}
