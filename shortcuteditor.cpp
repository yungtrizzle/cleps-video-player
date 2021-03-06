/******************************************************************************
*   Cleps Video Player. Simply a media player, no more, no less.              *
*   Copyright (C) 2014  Eshton Robateau <eshtonrob@gmail.com>                 *
*                                                                             *
*    This program is free software: you can redistribute it and/or modify     *
*    it under the terms of the GNU General Public License as published by     *
*    the Free Software Foundation, either version 3 of the License, or        *
*    (at your option) any later version.                                      *
*                                                                             *
*    This program is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*    GNU General Public License for more details.                             *
*                                                                             *
*    You should have received a copy of the GNU General Public License        *
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
*******************************************************************************/

#include "shortcuteditor.h"
#include <QBoxLayout>


shortcutEditor::shortcutEditor(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(250,100);

    msg = new QLabel(tr("<i>Enter New ShortCut</i>"));
    editor = new QKeySequenceEdit(this);
    ok = new QPushButton(tr("Ok"));
    QBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(msg);
    vlay->addWidget(editor);
    vlay->addWidget(ok);
    setLayout(vlay);

    connect(editor,SIGNAL(keySequenceChanged(QKeySequence)),this->parent(),SLOT(readSequence(QKeySequence)));
    connect(ok,SIGNAL(clicked()),this, SLOT(close()));
}
