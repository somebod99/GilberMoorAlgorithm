#include "decodewidjet.h"
#include "ui_decodewidjet.h"

#include "GilberMoorAlgorithm.h"
#include <fstream>

#include <QFileDialog>
#include <QMessageBox>

DecodeWidjet::DecodeWidjet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecodeWidjet)
{
    ui->setupUi(this);

    connect(ui->alphabet_file_dialog_button, SIGNAL(clicked()), this, SLOT(slotOnClickAlphButton()));
    connect(ui->encode_seq_file_dialog_button, SIGNAL(clicked()), this, SLOT(slotOnClickAlphButton()));
    connect(ui->decode_button, SIGNAL(clicked()), this, SLOT(slotOnClickDecodeButton()));
}

DecodeWidjet::~DecodeWidjet()
{
    delete ui;
}

void DecodeWidjet::slotOnClickAlphButton()
{
    if(sender()->objectName() == "alphabet_file_dialog_button")
        ui->alphabet_edit->setText(QFileDialog::getOpenFileName(this, "Files"));
    else
        ui->encode_seq_edit->setText(QFileDialog::getOpenFileName(this, "Files"));
}

void DecodeWidjet::slotOnClickDecodeButton()
{
    try
    {
        if(ui->alphabet_edit->text() == "")
            throw "Error: enter the path to the file with the alphabet";
        if(ui->encode_seq_edit->text() == "")
            throw "Error: enter the path to the file with the encoded sequence";

        GilberMoorDecoder decoder(GilberMoorEncoder(ui->alphabet_edit->text().toStdString()));
        std::string res = decoder.DecodeFromBinaryFile(ui->encode_seq_edit->text().toStdString());

        QMessageBox::information(this, "", "Sequence decoded successfully");

        std::ofstream file("out.txt");
        file << res;
        file.close();
    }
    catch (const char* er)
    {
        QMessageBox::critical(this,"ERROR", er);
    }
}
