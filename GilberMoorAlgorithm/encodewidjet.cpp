#include "encodewidjet.h"
#include "ui_encodewidjet.h"

#include "GilberMoorAlgorithm.h"
#include <sstream>

#include <QFileDialog>
#include <QMessageBox>

EncodeWidjet::EncodeWidjet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EncodeWidjet)
{
    ui->setupUi(this);

    connect(ui->alphabet_file_dialog_button, SIGNAL(clicked()), this, SLOT(slotOnClickAlphButton()));
    connect(ui->encode_seq_file_dialog_button, SIGNAL(clicked()), this, SLOT(slotOnClickAlphButton()));
    connect(ui->encode_button, SIGNAL(clicked()), this, SLOT(slotOnClickEncodeButton()));
}

EncodeWidjet::~EncodeWidjet()
{
    delete ui;
}

void EncodeWidjet::slotOnClickAlphButton()
{
    if(sender()->objectName() == "alphabet_file_dialog_button")
        ui->alphabet_edit->setText(QFileDialog::getOpenFileName(this, "Files"));
    else
        ui->encode_seq_edit->setText(QFileDialog::getOpenFileName(this, "Files"));
}

void EncodeWidjet::slotOnClickEncodeButton()
{
    try
    {
        if(ui->alphabet_edit->text() == "")
            throw "Error: enter the path to the file with the alphabet";
        if(ui->encode_seq_edit->text() == "")
            throw "Error: enter the path to the file with the sequence to encode";

        GilberMoorEncoder encoder(ui->alphabet_edit->text().toStdString());
        std::vector<bool> res = encoder.EncodeFromTxtFile(ui->encode_seq_edit->text().toStdString());
        WriteToBinaryFile("out.bin", res);

        std::map<std::string, std::vector<bool>> codes = encoder.getCodes();
        std::stringstream str_stream;

        for(const auto& key_value : codes)
        {
            str_stream << key_value.first << ": ";

            for(const auto& bit : key_value.second)
            {
                str_stream << bit;
            }

            str_stream << '\n';
        }

        ui->codes_text_browser->setText(QString::fromStdString(str_stream.str()));
        ui->l_value_label->setText(QString::number(encoder.getL()));
        ui->r_value_label->setText(QString::number(encoder.getR()));
        ui->kraft_value_label->setText(QString::number(encoder.getKraftInequality()) + " <= 1");

        QMessageBox::information(this, "", "Sequence encoded successfully");
    }
    catch(const char* er)
    {
        QMessageBox::critical(this,"ERROR", er);
    }
}
