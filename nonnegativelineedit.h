#ifndef NONNEGATIVELINEEDIT_H
#define NONNEGATIVELINEEDIT_H

#include <QLineEdit>
#include <QValidator>

class NonNegativeValidator : public QValidator {
public:
    QValidator::State validate(QString &input, int &pos) const override {
        bool ok;
        int value = input.toInt(&ok);
        if (ok && value >= 0) {
            return QValidator::Acceptable;
        } else {
            return QValidator::Invalid;
        }
    }
};

class NonNegativeLineEdit : public QLineEdit {
public:
    NonNegativeLineEdit(QWidget *parent = nullptr)
        : QLineEdit(parent) {
        setValidator(new NonNegativeValidator);
    }
};

#endif // NONNEGATIVELINEEDIT_H
