﻿#include "passphrasedlg.h"

passphrase::passphrase(QWidget *parent, PassphraseDlgInfo *ptr)
	: QDialog(parent)
{
    ui.setupUi(this);

	setWindowTitle(ptr->caption);
    ui.lineEdit->setEchoMode(QLineEdit::Password);
    ui.lineEdit->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    ui.label_2->setText(ptr->text);
	if (!ptr->saveAvailable) {
		ui.checkBox->setChecked(false);
		ui.checkBox->setEnabled(false);
	}

    infoPtr_ = ptr;
}

passphrase::~passphrase()
{
}

void passphrase::accept()
{
	QString s = ui.lineEdit->text();
	bool c = ui.checkBox->isChecked();
	if (infoPtr_ != NULL) {
        std::string ss = s.toStdString();
        const char *s_const = ss.c_str();
        *infoPtr_->passphrase = _strdup(s_const);
	}
    infoPtr_->save = c == true ? 1 : 0;
    QDialog::accept();
}

void passphrase::reject()
{
	*infoPtr_->passphrase = NULL;
	QDialog::reject();
}

extern "C"
char *pin_dlg(const wchar_t *text, const wchar_t *caption, HWND hWnd, BOOL *pSavePassword)
{
	char *_passphrase;
	struct PassphraseDlgInfo pps;
	pps.passphrase = &_passphrase;
	std::string sText = QString::fromStdWString(text).toStdString();
	std::string sCaption = QString::fromStdWString(caption).toStdString();
	pps.caption = sCaption.c_str();
	pps.text = sText.c_str();
	if (pSavePassword == NULL) {
		pps.save = 0;
		pps.saveAvailable = false;
	} else {
		pps.save = *pSavePassword;
		pps.saveAvailable = true;
	}

	DIALOG_RESULT_T r = passphraseDlg(&pps);
	if (r == DIALOG_RESULT_CANCEL) {
		if (pSavePassword != NULL) {
			*pSavePassword = FALSE;
		}
		return NULL;
	}

	if (pSavePassword != NULL) {
		*pSavePassword = pps.save;
	}
	return _passphrase;
}

// Local Variables:
// mode: c++
// coding: utf-8-with-signature
// tab-width: 4
// End:
