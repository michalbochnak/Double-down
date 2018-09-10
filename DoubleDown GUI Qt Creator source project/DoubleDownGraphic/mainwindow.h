#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // my function declarations

private slots:
    void on_pushButton_A_clicked();

    void on_pushButton_E_clicked();

    void on_pushButton_I_clicked();

    void on_pushButton_O_clicked();

    void on_pushButton_a_clicked();

    void on_pushButton_e_clicked();

    void on_pushButton_i_clicked();

    void on_pushButton_o_clicked();

    void on_pushButton_B_clicked();

    void on_pushButton_C_clicked();

    void on_pushButton_D_clicked();

    void on_pushButton_F_clicked();

    void on_pushButton_b_clicked();

    void on_pushButton_c_clicked();

    void on_pushButton_d_clicked();

    void on_pushButton_f_clicked();





    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_1_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_clicked();



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
