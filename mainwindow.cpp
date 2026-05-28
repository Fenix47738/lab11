#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Ініціалізація інтерфейсу користувача
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    // Звільнення виділених ресурсів (інтерфейс)
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    // декларації змінних для подальших обчислень
    float a, b, avg;
    bool ok;

    // зчитування значень з полів введення (`le_a`, `le_b`)
    // Заувага: кожен виклик toFloat записує результат у `ok`.
    a = ui->le_a->text().toFloat(&ok);
    b = ui->le_b->text().toFloat(&ok);

    // якщо останнє перетворення пройшло успішно — виводимо середнє
    if (ok) {
        // обчислення середнього арифметичного
        avg = (a + b) / 2.0f;
        // виведення результату у віджет, який показує суму/результат
        ui->l_sum->setText(QString::number(avg));
    }
    else {
        // обробка некоректного введення: показ повідомлення та скидання полів
        QMessageBox::warning(0, "Error", "Wrong value(s)!!!");
        ui->le_a->clear();
        ui->le_b->clear();
        ui->l_sum->setText("0");
        ui->le_a->setFocus();
    }
}

void MainWindow::on_actionLoad_from_file_triggered()
{
    // Зчитування даних з файлу: відкриваємо діалог вибору файлу
    QString filePath = QFileDialog::getOpenFileName(this, "Open file",
                                                    QDir::currentPath(), "Текстові файли (*.txt);;Усі файли (*)");

    // перевіряємо чи користувач вказав файл
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        // намагаємося відкрити файл лише для читання в текстовому режимі
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // не вдалося відкрити — показуємо попередження
            QMessageBox::warning(0, "Error", "Can not open file!");
        }
        else {
            // читаємо три рядки/значення з файлу та заповнюємо поля інтерфейсу
            QTextStream in(&file);
            QString sa, sb, sum;
            in >> sa >> sb >> sum;
            ui->le_a->setText(sa);
            ui->le_b->setText(sb);
            ui->l_sum->setText(sum);
            file.close();
        }
    }
    else {
        // користувач відмінив діалог або не вказав ім'я файлу
        QMessageBox::warning(0, "Error", "Unknown filename!");
    }
}
void MainWindow::on_actionSave_to_file_triggered()
{
    // Збереження поточних значень у файл: відкриваємо діалог збереження
    QString filePath = QFileDialog::getSaveFileName(this, "Save file",
                                                    QDir::currentPath(), "Текстові файли (*.txt);;Усі файли (*)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        // намагаємося відкрити файл для запису в текстовому режимі
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // критична помилка при відкритті файлу — повідомляємо користувача
            QMessageBox::critical(nullptr, "Error",
                                  "Can not open file:\n" + file.errorString());
        }
        else {
            // записуємо значення по рядках: перше поле, друге поле, результат
            QTextStream out(&file);
            out << ui->le_a->text() << "\n"
                << ui->le_b->text() << "\n"
                << ui->l_sum ->toPlainText();
            file.close();
        }
    }
    else {
        // користувач не вказав ім'я файлу або відмінив діалог
        QMessageBox::warning(0, "Error", "Unknown filename!");
    }
}
