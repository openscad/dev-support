/****************************************************************************
 **
 ** Copyright (C) 2016 The Qt Company Ltd.
 ** Contact: https://www.qt.io/licensing/
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and The Qt Company. For licensing terms
 ** and conditions see https://www.qt.io/terms-conditions. For further
 ** information use the contact form at https://www.qt.io/contact-us.
 **
 ** BSD License Usage
 ** Alternatively, you may use this file under the terms of the BSD license
 ** as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of The Qt Company Ltd nor the names of its
 **     contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include <QtWidgets>

#include "mainwindow.h"

MainWindow::MainWindow() : textEdit(new QsciScintilla)
{
	setCentralWidget(textEdit);
	resize(QDesktopWidget().availableGeometry(this).size() * 0.4);

	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	font.setPointSize(28);

	textEdit->setFont(font);
	textEdit->setBackgroundRole(QPalette::ColorRole::Dark);
	textEdit->setWrapMode(QsciScintilla::WrapMode::WrapWord);
	textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
	textEdit->setAutoCompletionFillupsEnabled(false);
	textEdit->setAutoCompletionFillups("(");
	textEdit->setAutoCompletionThreshold(1);
	textEdit->setText("\n\n\n\n\n"
					"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque imperdiet est nulla, non efficitur nisl mollis eu. Nulla vitae quam augue. Morbi ipsum risus, facilisis id velit vel, dapibus molestie dolor. Aliquam eu cursus quam. Donec dictum velit id erat dictum semper. Aenean ipsum magna, tincidunt a lacus sed, convallis tincidunt magna. Nam sed enim id ipsum dictum auctor a quis justo. Proin porttitor mauris in elit accumsan, a dapibus nisl pretium. Nullam sit amet elit ante. Vivamus eu nunc lectus. Cras in efficitur erat. Aliquam vestibulum semper sem eu gravida.\n\n"
					"Someverylongwordsarealsotherefortesting Someotherlongwordtoinclude Someeventlongerwordthatshouldbetoolongforthewindow Somemorestuff\n\n"
					"Thistextlineiseventoolongforthewholedesktopscreensizetofitthepopupsoweexpectsomehorizontalscrollbarstoappear Thisother Thisx1 Thisx2 Thisx3 Thisx4 Thisx5\n\n"
					"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam dolor arcu, congue in diam id, rhoncus volutpat dui. Suspendisse et tincidunt quam. Proin dolor ex, tristique sit amet molestie in, vehicula ac sem. Donec dapibus sed dui pretium semper. Donec gravida dapibus felis et rutrum. Sed consequat enim vel urna pretium congue. Maecenas ipsum libero, lacinia eu ultricies sed, tempor quis sem. Mauris porttitor justo vel purus scelerisque faucibus. Sed nec scelerisque nulla, in fringilla nulla. Nullam aliquam auctor dui, vel malesuada ex. Vivamus posuere lacinia ante, vel venenatis tortor imperdiet eu.\n\n"
					"Curabitur placerat elit eget elit accumsan, sit amet tempus orci ullamcorper. Duis sagittis finibus mauris et aliquam. Cras efficitur massa sit amet ligula sagittis, id viverra quam bibendum. Curabitur consectetur augue venenatis hendrerit condimentum. Donec sollicitudin erat aliquet ex eleifend, quis posuere mauris vehicula. Morbi volutpat libero eget laoreet ornare. Proin sed congue massa. Sed non odio non velit volutpat luctus quis sed risus. Fusce accumsan ipsum id tellus ultrices ornare. Suspendisse luctus eu magna sagittis vestibulum. Quisque tortor odio, egestas non commodo ut, pellentesque ut elit. Nunc in maximus ligula.\n\n"
					"Proin a justo vulputate, vehicula nulla vitae, placerat risus. Duis ac turpis velit. In sodales quam augue, id dictum eros condimentum sed. Pellentesque in sapien sed massa volutpat viverra eget nec erat. Pellentesque non egestas ligula. Aliquam a elit iaculis, lobortis nunc ac, semper mauris. In venenatis semper porta. Donec convallis dolor felis, in mattis eros egestas ac. Maecenas dui neque, euismod non aliquet sit amet, malesuada a turpis. Nunc convallis nec risus vitae dapibus. Nulla sed dolor fringilla, facilisis ex in, fermentum tortor. Aliquam id augue tempor, egestas sapien eu, tempus metus. Integer volutpat velit vitae ante consectetur, vel porta metus malesuada. Integer interdum justo varius neque euismod, et volutpat libero tincidunt. In tristique quis lacus tincidunt ultrices.\n\n"
					"Praesent vitae sapien sed enim laoreet sollicitudin tempus eu massa. Sed ultrices, nisi vitae venenatis elementum, dolor sem fermentum orci, at pretium mi diam eu purus. Donec pulvinar, enim eget consectetur vestibulum, felis leo tempor velit, eget varius leo nisi a quam. Praesent eleifend justo lacus, non sollicitudin nunc viverra eget. Nulla tortor metus, pretium vel sollicitudin id, blandit id velit. Mauris vestibulum suscipit urna, finibus cursus purus gravida sit amet. Ut orci diam, mollis a eros sed, porttitor rhoncus risus. Proin lobortis dictum dapibus. Vestibulum vel ex ut tortor rhoncus bibendum non vel ligula. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.");
	textEdit->setCursorPosition(1, 0);

#ifdef Q_OS_MAC
	const unsigned long modifier = Qt::META;
#else
	const unsigned long modifier = Qt::CTRL;
#endif

	QShortcut *shortcutAutocomplete = new QShortcut(modifier | Qt::Key_Space, this);
	connect(shortcutAutocomplete, &QShortcut::activated, [ = ](){textEdit->autoCompleteFromAll();});
}
