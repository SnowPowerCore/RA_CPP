#pragma once

#include <QtWidgets/QMainWindow>

#include <memory>

namespace Ui
{
    class MainWindow;
}

/**
 * @class MainWindow
 * @brief Главное окно приложения.
 */
class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    Q_DISABLE_COPY(MainWindow)

private:
    // NOTE: Уникально владеем указателем на сгенерированное по UI-форме представление окна всё время жизни объекта.
    std::unique_ptr<Ui::MainWindow> ui_;
};
