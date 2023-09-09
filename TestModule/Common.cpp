#include "Common.h"

QString CCommon::GetStandardIconDescription(int iconIndex)
{
    switch (iconIndex)
    {
    case 1: return "QStyle::SP_TitleBarMinButton\r\nMinimize button on title bars (e.g., in QMdiSubWindow)."; break;
    case 0: return "QStyle::SP_TitleBarMenuButton\r\nMenu button on a title bar."; break;
    case 2: return "QStyle::SP_TitleBarMaxButton\r\nMaximize button on title bars."; break;
    case 3: return "QStyle::SP_TitleBarCloseButton\r\nClose button on title bars."; break;
    case 4: return "QStyle::SP_TitleBarNormalButton\r\nNormal (restore) button on title bars."; break;
    case 5: return "QStyle::SP_TitleBarShadeButton\r\nShade button on title bars."; break;
    case 6: return "QStyle::SP_TitleBarUnshadeButton\r\nUnshade button on title bars."; break;
    case 7: return "QStyle::SP_TitleBarContextHelpButton\r\nThe Context help button on title bars."; break;
    case 9: return "QStyle::SP_MessageBoxInformation\r\nThe \"information\" icon."; break;
    case 10: return "QStyle::SP_MessageBoxWarning\r\nThe \"warning\" icon."; break;
    case 11: return "QStyle::SP_MessageBoxCritical\r\nThe \"critical\" icon."; break;
    case 12: return "QStyle::SP_MessageBoxQuestion\r\nThe \"question\" icon."; break;
    case 13: return "QStyle::SP_DesktopIcon\r\nThe \"desktop\" icon."; break;
    case 14: return "QStyle::SP_TrashIcon\r\nThe \"trash\" icon."; break;
    case 15: return "QStyle::SP_ComputerIcon\r\nThe \"My computer\" icon."; break;
    case 16: return "QStyle::SP_DriveFDIcon\r\nThe floppy icon."; break;
    case 17: return "QStyle::SP_DriveHDIcon\r\nThe harddrive icon."; break;
    case 18: return "QStyle::SP_DriveCDIcon\r\nThe CD icon."; break;
    case 19: return "QStyle::SP_DriveDVDIcon\r\nThe DVD icon."; break;
    case 20: return "QStyle::SP_DriveNetIcon\r\nThe network icon."; break;
    case 56: return "QStyle::SP_DirHomeIcon\r\nThe home directory icon."; break;
    case 21: return "QStyle::SP_DirOpenIcon\r\nThe open directory icon."; break;
    case 22: return "QStyle::SP_DirClosedIcon\r\nThe closed directory icon."; break;
    case 38: return "QStyle::SP_DirIcon\r\nThe directory icon."; break;
    case 23: return "QStyle::SP_DirLinkIcon\r\nThe link to directory icon."; break;
    case 24: return "QStyle::SP_DirLinkOpenIcon\r\nThe link to open directory icon."; break;
    case 25: return "QStyle::SP_FileIcon\r\nThe file icon."; break;
    case 26: return "QStyle::SP_FileLinkIcon\r\nThe link to file icon."; break;
    case 29: return "QStyle::SP_FileDialogStart\r\nThe \"start\" icon in a file dialog."; break;
    case 30: return "QStyle::SP_FileDialogEnd\r\nThe \"end\" icon in a file dialog."; break;
    case 31: return "QStyle::SP_FileDialogToParent\r\nThe \"parent directory\" icon in a file dialog."; break;
    case 32: return "QStyle::SP_FileDialogNewFolder\r\nThe \"create new folder\" icon in a file dialog."; break;
    case 33: return "QStyle::SP_FileDialogDetailedView\r\nThe detailed view icon in a file dialog."; break;
    case 34: return "QStyle::SP_FileDialogInfoView\r\nThe file info icon in a file dialog."; break;
    case 35: return "QStyle::SP_FileDialogContentsView\r\nThe contents view icon in a file dialog."; break;
    case 36: return "QStyle::SP_FileDialogListView\r\nThe list view icon in a file dialog."; break;
    case 37: return "QStyle::SP_FileDialogBack\r\nThe back arrow in a file dialog."; break;
    case 8: return "QStyle::SP_DockWidgetCloseButton\r\nClose button on dock windows (see also QDockWidget)."; break;
    case 27: return "QStyle::SP_ToolBarHorizontalExtensionButton\r\nExtension button for horizontal toolbars."; break;
    case 28: return "QStyle::SP_ToolBarVerticalExtensionButton\r\nExtension button for vertical toolbars."; break;
    case 39: return "QStyle::SP_DialogOkButton\r\nIcon for a standard OK button in a QDialogButtonBox."; break;
    case 40: return "QStyle::SP_DialogCancelButton\r\nIcon for a standard Cancel button in a QDialogButtonBox."; break;
    case 41: return "QStyle::SP_DialogHelpButton\r\nIcon for a standard Help button in a QDialogButtonBox."; break;
    case 42: return "QStyle::SP_DialogOpenButton\r\nIcon for a standard Open button in a QDialogButtonBox."; break;
    case 43: return "QStyle::SP_DialogSaveButton\r\nIcon for a standard Save button in a QDialogButtonBox."; break;
    case 44: return "QStyle::SP_DialogCloseButton\r\nIcon for a standard Close button in a QDialogButtonBox."; break;
    case 45: return "QStyle::SP_DialogApplyButton\r\nIcon for a standard Apply button in a QDialogButtonBox."; break;
    case 46: return "QStyle::SP_DialogResetButton\r\nIcon for a standard Reset button in a QDialogButtonBox."; break;
    case 47: return "QStyle::SP_DialogDiscardButton\r\nIcon for a standard Discard button in a QDialogButtonBox."; break;
    case 48: return "QStyle::SP_DialogYesButton\r\nIcon for a standard Yes button in a QDialogButtonBox."; break;
    case 49: return "QStyle::SP_DialogNoButton\r\nIcon for a standard No button in a QDialogButtonBox."; break;
    case 50: return "QStyle::SP_ArrowUp\r\nIcon arrow pointing up."; break;
    case 51: return "QStyle::SP_ArrowDown\r\nIcon arrow pointing down."; break;
    case 52: return "QStyle::SP_ArrowLeft\r\nIcon arrow pointing left."; break;
    case 53: return "QStyle::SP_ArrowRight\r\nIcon arrow pointing right."; break;
    case 54: return "QStyle::SP_ArrowBack\r\nEquivalent to SP_ArrowLeft when the current layout direction is Qt::LeftToRight, otherwise SP_ArrowRight."; break;
    case 55: return "QStyle::SP_ArrowForward\r\nEquivalent to SP_ArrowRight when the current layout direction is Qt::LeftToRight, otherwise SP_ArrowLeft."; break;
    case 57: return "QStyle::SP_CommandLink\r\nIcon used to indicate a Vista style command link glyph."; break;
    case 58: return "QStyle::SP_VistaShield\r\nIcon used to indicate UAC prompts on Windows Vista. This will return a null pixmap or icon on all other platforms."; break;
    case 59: return "QStyle::SP_BrowserReload\r\nIcon indicating that the current page should be reloaded."; break;
    case 60: return "QStyle::SP_BrowserStop\r\nIcon indicating that the page loading should stop."; break;
    case 61: return "QStyle::SP_MediaPlay\r\nIcon indicating that media should begin playback."; break;
    case 62: return "QStyle::SP_MediaStop\r\nIcon indicating that media should stop playback."; break;
    case 63: return "QStyle::SP_MediaPause\r\nIcon indicating that media should pause playback."; break;
    case 64: return "QStyle::SP_MediaSkipForward\r\nIcon indicating that media should skip forward."; break;
    case 65: return "QStyle::SP_MediaSkipBackward\r\nIcon indicating that media should skip backward."; break;
    case 66: return "QStyle::SP_MediaSeekForward\r\nIcon indicating that media should seek forward."; break;
    case 67: return "QStyle::SP_MediaSeekBackward\r\nIcon indicating that media should seek backward."; break;
    case 68: return "QStyle::SP_MediaVolume\r\nIcon indicating a volume control."; break;
    case 69: return "QStyle::SP_MediaVolumeMuted\r\nIcon indicating a muted volume control."; break;
    case 70: return "QStyle::SP_LineEditClearButton\r\nIcon for a standard clear button in a QLineEdit. This enum value was added in Qt 5.2."; break;
    case 71: return "QStyle::SP_DialogYesToAllButton\r\nIcon for a standard YesToAll button in a QDialogButtonBox. This enum value was added in Qt 5.14."; break;
    case 72: return "QStyle::SP_DialogNoToAllButton\r\nIcon for a standard NoToAll button in a QDialogButtonBox. This enum value was added in Qt 5.14."; break;
    case 73: return "QStyle::SP_DialogSaveAllButton\r\nIcon for a standard SaveAll button in a QDialogButtonBox. This enum value was added in Qt 5.14."; break;
    case 74: return "QStyle::SP_DialogAbortButton\r\nIcon for a standard Abort button in a QDialogButtonBox. This enum value was added in Qt 5.14."; break;
    case 75: return "QStyle::SP_DialogRetryButton\r\nIcon for a standard Retry button in a QDialogButtonBox. This enum value was added in Qt 5.14."; break;
    case 76: return "QStyle::SP_DialogIgnoreButton\r\nIcon for a standard Ignore button in a QDialogButtonBox. This enum value was added in Qt 5.14."; break;
    case 77: return "QStyle::SP_RestoreDefaultsButton\r\nIcon for a standard RestoreDefaults button in a QDialogButtonBox. This enum value was added in Qt 5.14."; break;
    default:
        break;
    }
    return QString();
}
