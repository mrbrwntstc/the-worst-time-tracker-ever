#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public /*methods*/:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void calculateTime();
  void reset();

private /*methods*/:
  void initializeProgressBar();
  void setDefaultTimeIn();
  void setDefaultLunchBreak();
  void populateComboBox();
  void resetDurationOfTimeLabel();
  void setDurationOfTimeLabel(double durationOfTime);

private /*fields*/:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
