#include "PCH/PCH.h"

#include "Dialog.h"

#include <windows.h>
#include <shobjidl_core.h>

namespace Volund
{
    std::string Dialog::OpenFolder()
    {
        return FileDialog(FOS_PICKFOLDERS);
    }

    std::string Dialog::OpenFile()
    {
        return FileDialog();
    }

    std::string Dialog::FileDialog(FILEOPENDIALOGOPTIONS Options)
    {
        std::string Output;

        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pFileOpen = NULL;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr))
            {
                // Show the Open dialog box.
                pFileOpen->SetOptions(Options);
                //pFileOpen->SetFileTypes(numTypes, cdfs);   //  choose file types to be displayed
                pFileOpen->SetTitle(L"Open File");         //  heading of dialog box
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        LPWSTR pTemp;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pTemp);
                        std::wstring Temp = pTemp;
                        Output = std::string(Temp.begin(), Temp.end());
                        if (SUCCEEDED(hr)) 
                        {
                            CoTaskMemFree(pTemp);
                        }
                        pItem->Release();
                    }
                }
                pFileOpen->Release();
            }
            CoUninitialize();
        }

        return Output;
    }
}