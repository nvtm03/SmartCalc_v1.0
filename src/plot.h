#ifndef PLOT_H
#define PLOT_H

#include <QVector>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Plot;
}
QT_END_NAMESPACE

class Plot : public QWidget {
  Q_OBJECT

 public:
  explicit Plot(QWidget *parent = nullptr);
  void draw_plot(char *text, double x_min_num, double x_max_num, double range_x,
                 double range_y);

  ~Plot();

 private slots:
  void on_pushButton_change_diapason_clicked();

 private:
  Ui::Plot *ui;
  double xBegin, xEnd, h, X;
  int N;

  char temp_str[255];
  QString text_str;

  QVector<double> x, y;
};

#endif  // PLOT_H
