#include <wx/wx.h>
#include <wx/fileconf.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "tsnsoft.xpm"

class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, wxT("Update .dev Paths"), wxDefaultPosition, wxSize(500, 300)) {
        // Инициализация пути к файлу конфигурации
        CONFIG_FILE = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath() + wxT("/config.ini");

        SetIcon(tsnsoft_xpm); // Установка иконки

        // Создаём основной сайзер
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

        // Поля для путей
        wxBoxSizer* compilerSizer = new wxBoxSizer(wxHORIZONTAL);
        compilerLabel = new wxStaticText(panel, wxID_ANY, wxT("Компилятор:"));
        compilerText = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400, -1), wxTE_READONLY);
        compilerButton = new wxButton(panel, wxID_ANY, wxT("Выбрать..."), wxDefaultPosition, wxSize(100, -1));
        compilerSizer->Add(compilerLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
        compilerSizer->Add(compilerText, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5); // Пропорция 1 для растяжки
        compilerSizer->Add(compilerButton, 0, wxALIGN_CENTER_VERTICAL);
        panelSizer->Add(compilerSizer, 0, wxEXPAND | wxALL, 5); // Добавлен wxEXPAND

        wxBoxSizer* wxWidgetsSizer = new wxBoxSizer(wxHORIZONTAL);
        wxWidgetsLabel = new wxStaticText(panel, wxID_ANY, wxT("wxWidgets:"));
        wxWidgetsText = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400, -1), wxTE_READONLY);
        wxWidgetsButton = new wxButton(panel, wxID_ANY, wxT("Выбрать..."), wxDefaultPosition, wxSize(100, -1));
        wxWidgetsSizer->Add(wxWidgetsLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
        wxWidgetsSizer->Add(wxWidgetsText, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5); // Пропорция 1 для растяжки
        wxWidgetsSizer->Add(wxWidgetsButton, 0, wxALIGN_CENTER_VERTICAL);
        panelSizer->Add(wxWidgetsSizer, 0, wxEXPAND | wxALL, 5); // Добавлен wxEXPAND

        wxBoxSizer* devFileSizer = new wxBoxSizer(wxHORIZONTAL);
        devFileLabel = new wxStaticText(panel, wxID_ANY, wxT("Файл .dev:"));
        devFileText = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(400, -1), wxTE_READONLY);
        devFileButton = new wxButton(panel, wxID_ANY, wxT("Выбрать..."), wxDefaultPosition, wxSize(100, -1));
        devFileSizer->Add(devFileLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
        devFileSizer->Add(devFileText, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5); // Пропорция 1 для растяжки
        devFileSizer->Add(devFileButton, 0, wxALIGN_CENTER_VERTICAL);
        panelSizer->Add(devFileSizer, 0, wxEXPAND | wxALL, 5); // Добавлен wxEXPAND

        // Кнопка для обновления
        updateButton = new wxButton(panel, wxID_ANY, wxT("Обновить файл .dev"));
        panelSizer->Add(updateButton, 0, wxALIGN_CENTER | wxALL, 10);

        // Установка сайзера панели
        panel->SetSizer(panelSizer);
        mainSizer->Add(panel, 1, wxEXPAND);
        SetSizerAndFit(mainSizer);

        // Загрузка конфигурации
        LoadConfig();

        // Привязка событий
        compilerButton->Bind(wxEVT_BUTTON, &MyFrame::OnSelectCompilerFolder, this);
        wxWidgetsButton->Bind(wxEVT_BUTTON, &MyFrame::OnSelectWxWidgetsFolder, this);
        devFileButton->Bind(wxEVT_BUTTON, &MyFrame::OnSelectDevFile, this);
        updateButton->Bind(wxEVT_BUTTON, &MyFrame::OnUpdateDevFile, this);
    }

private:
    wxString CONFIG_FILE; // Член класса
    wxStaticText* compilerLabel;
    wxTextCtrl* compilerText;
    wxButton* compilerButton;
    wxStaticText* wxWidgetsLabel;
    wxTextCtrl* wxWidgetsText;
    wxButton* wxWidgetsButton;
    wxStaticText* devFileLabel;
    wxTextCtrl* devFileText;
    wxButton* devFileButton;
    wxButton* updateButton;

    void LoadConfig() {
        wxFileConfig config(wxEmptyString, wxEmptyString, CONFIG_FILE);
        wxString compilerFolder, wxWidgetsFolder, devFile;

        if (config.Read(wxT("Paths/CompilerFolder"), &compilerFolder) && wxFileName::DirExists(compilerFolder)) {
            compilerText->SetValue(compilerFolder);
        }
        if (config.Read(wxT("Paths/WxWidgetsFolder"), &wxWidgetsFolder) && wxFileName::DirExists(wxWidgetsFolder)) {
            wxWidgetsText->SetValue(wxWidgetsFolder);
        }
        if (config.Read(wxT("Paths/DevFile"), &devFile) && wxFileName::FileExists(devFile)) {
            devFileText->SetValue(devFile);
        }
    }

    void SaveConfig() {
        wxFileConfig config(wxEmptyString, wxEmptyString, CONFIG_FILE);
        config.Write(wxT("Paths/CompilerFolder"), compilerText->GetValue());
        config.Write(wxT("Paths/WxWidgetsFolder"), wxWidgetsText->GetValue());
        config.Write(wxT("Paths/DevFile"), devFileText->GetValue());
        config.Flush();
    }

    void OnSelectCompilerFolder(wxCommandEvent& event) {
        wxDirDialog dlg(this, wxT("Выберите папку с компилятором"), compilerText->GetValue(),
                        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
        if (dlg.ShowModal() == wxID_OK) {
            compilerText->SetValue(dlg.GetPath());
        }
    }

    void OnSelectWxWidgetsFolder(wxCommandEvent& event) {
        wxDirDialog dlg(this, wxT("Выберите папку с wxWidgets"), wxWidgetsText->GetValue(),
                        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
        if (dlg.ShowModal() == wxID_OK) {
            wxWidgetsText->SetValue(dlg.GetPath());
        }
    }

    void OnSelectDevFile(wxCommandEvent& event) {
        wxFileDialog dlg(this, wxT("Выберите файл .dev"), wxEmptyString, wxEmptyString,
                         wxT("Dev files (*.dev)|*.dev"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (!devFileText->GetValue().IsEmpty()) {
            dlg.SetPath(devFileText->GetValue());
        }
        if (dlg.ShowModal() == wxID_OK) {
            devFileText->SetValue(dlg.GetPath());
        }
    }

    void OnUpdateDevFile(wxCommandEvent& event) {
        // Проверка заполненности полей
        if (compilerText->GetValue().IsEmpty()) {
            wxMessageBox(wxT("Пожалуйста, выберите папку компилятора."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            return;
        }
        if (wxWidgetsText->GetValue().IsEmpty()) {
            wxMessageBox(wxT("Пожалуйста, выберите папку wxWidgets."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            return;
        }
        if (devFileText->GetValue().IsEmpty()) {
            wxMessageBox(wxT("Пожалуйста, выберите файл .dev."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            return;
        }

        // Проверка существования путей
        if (!wxFileName::DirExists(compilerText->GetValue())) {
            wxMessageBox(wxT("Папка компилятора не существует."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            return;
        }
        if (!wxFileName::DirExists(wxWidgetsText->GetValue())) {
            wxMessageBox(wxT("Папка wxWidgets не существует."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            return;
        }
        if (!wxFileName::FileExists(devFileText->GetValue())) {
            wxMessageBox(wxT("Файл .dev не существует."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            return;
        }

        // Формирование путей с прямыми слэшами
        wxString wxInclude = wxWidgetsText->GetValue() + wxT("/include");
        wxString wxLibMswud = wxWidgetsText->GetValue() + wxT("/lib/gcc_lib/mswud");
        wxString wxLib = wxWidgetsText->GetValue() + wxT("/lib/gcc_lib");
        wxInclude.Replace(wxT("\\"), wxT("/"));
        wxLibMswud.Replace(wxT("\\"), wxT("/"));
        wxLib.Replace(wxT("\\"), wxT("/"));

        // Чтение и обновление файла .dev
        wxTextFile file(devFileText->GetValue());
        if (!file.Open()) {
            wxMessageBox(wxT("Не удалось открыть файл .dev."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            return;
        }

        wxArrayString lines;
        for (size_t i = 0; i < file.GetLineCount(); ++i) {
            wxString line = file.GetLine(i);
            if (line.StartsWith(wxT("Includes ="))) {
                lines.Add(wxString::Format(wxT("Includes = %s;%s"), wxInclude, wxLibMswud));
            } else if (line.StartsWith(wxT("Libs ="))) {
                lines.Add(wxString::Format(wxT("Libs = %s"), wxLib));
            } else {
                lines.Add(line);
            }
        }

        file.Clear();
        for (const auto& line : lines) {
            file.AddLine(line);
        }
        if (!file.Write()) {
            wxMessageBox(wxT("Не удалось записать файл .dev."), wxT("Ошибка"), wxOK | wxICON_ERROR);
            file.Close();
            return;
        }
        file.Close();

        // Сохранение конфигурации
        SaveConfig();

        wxMessageBox(wxT("Файл .dev успешно обновлён!"), wxT("Успех"), wxOK | wxICON_INFORMATION);
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() override {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        frame->Centre();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);