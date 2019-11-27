#pragma once

#include <QtWidgets/QMainWindow>

#include <memory>

namespace Ui
{
    class MainWindow;
}

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Q_DISABLE_COPY(MainWindow)

private:
    std::unique_ptr<Ui::MainWindow> ui_;
};
