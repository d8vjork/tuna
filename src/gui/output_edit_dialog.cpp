/* output_edit_dialog.cpp created on 2019.9.18
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * github.com/univrsal/
 *
 */
#include "output_edit_dialog.hpp"
#include "ui_output_edit_dialog.h"
#include "tuna_gui.hpp"
#include "../util/constants.hpp"
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

output_edit_dialog::output_edit_dialog(edit_mode m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::output_edit_dialog),
    m_mode(m)
{
    ui->setupUi(this);
    ui->txt_format->setText(T_SONG_FORMAT_DEFAULT);
    m_tuna = dynamic_cast<tuna_gui*>(parent);

    if (m == edit_mode::modify) {
        QString format, path;
        m_tuna->get_selected_output(format, path);
        ui->txt_format->setText(format);
        ui->txt_path->setText(path);
    }
}

output_edit_dialog::~output_edit_dialog()
{
    delete ui;
}

static inline bool is_valid_file(const QString& file)
{
    QFileInfo info(file);
    return info.isWritable() && info.isFile();
}

void output_edit_dialog::on_buttonBox_accepted()
{
    if (ui->txt_format->text().isEmpty() || !is_valid_file(ui->txt_path->text())) {
        QMessageBox::warning(this, T_OUTPUT_ERROR_TITLE, T_OUTPUT_ERROR);
        return; /* Nothing to do */
    }

    if (m_mode == edit_mode::create) {
        m_tuna->add_output(ui->txt_format->text(), ui->txt_path->text());
    } else {
        m_tuna->edit_output(ui->txt_format->text(), ui->txt_path->text());
    }
}

void output_edit_dialog::on_pushButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(
        this, tr(T_SELECT_SONG_FILE), QDir::home().path(), tr(FILTER("Text file", "*.txt")));
    ui->txt_path->setText(path);
}