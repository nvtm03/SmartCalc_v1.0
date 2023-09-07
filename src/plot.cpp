#include "plot.h"

#include "qcustomplot.h"
#include "ui_plot.h"

extern "C" {
#include "s21_smart_calc.h"
}

Plot::Plot(QWidget* parent) : QWidget(parent), ui(new Ui::Plot) {
  ui->setupUi(this);
}

Plot::~Plot() { delete ui; }

void Plot::draw_plot(char* text, double x_min, double x_max, double min_y,
                     double max_y) {
  ui->widget->clearGraphs();
  strcpy(temp_str, text);

  ui->min_x_2->setValue(x_min);
  ui->max_x_2->setValue(x_max);
  ui->min_y_2->setValue(min_y);
  ui->max_y_2->setValue(max_y);

  xBegin = x_min;
  xEnd = x_max;

  ui->widget->xAxis->setRange(x_min - 2, x_max + 2);
  ui->widget->yAxis->setRange(min_y - 2, max_y + 2);

  h = 0.1;

  N = (xEnd - xBegin) / h + 2;
  int ExitCode = 0;
  for (X = xBegin; X < xEnd; X += h) {
    double result = 0.0;
    ExitCode = s21_smart_calc(text, X, &result);

    if (result >= min_y && result <= max_y && !ExitCode) {
      x.push_back(X);
      y.push_back(result);
    }
  }
  ui->widget->addGraph();
  ui->widget->graph(0)->addData(x, y);

  ui->widget->setInteraction(QCP::iRangeZoom, true);
  ui->widget->setInteraction(QCP::iRangeDrag, true);

  ui->widget->replot();

  x.clear();
  y.clear();
}

void Plot::on_pushButton_change_diapason_clicked() {
  double x_min_num = ui->min_x_2->text().toDouble();
  double x_max_num = ui->max_x_2->text().toDouble();
  double range_x = ui->min_y_2->text().toDouble();
  double range_y = ui->max_y_2->text().toDouble();
  Plot::draw_plot(temp_str, x_min_num, x_max_num, range_x, range_y);
}
