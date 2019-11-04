#ifndef CHESSBOARDCONFIG_H
#define CHESSBOARDCONFIG_H


class ChessBoardConfig
{
public:
    ChessBoardConfig();
    int boardW;//棋盘格的##横轴##方向的角点个数
    int boardH;//棋盘格的##纵轴##方向的角点个数
    double dx;//棋盘格的##横轴##单个格子的长度
    double dy;//棋盘格的##纵轴##单个格子的长度
};

#endif // CHESSBOARDCONFIG_H
