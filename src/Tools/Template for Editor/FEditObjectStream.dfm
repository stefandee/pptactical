object FormEditObjectStream: TFormEditObjectStream
  Left = 329
  Top = 240
  Width = 404
  Height = 640
  Caption = 'Object Stream Editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 396
    Height = 29
    Caption = 'ToolBar'
    TabOrder = 0
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Caption = 'Erase'
      ImageIndex = 0
      ParentShowHint = False
      ShowHint = False
    end
    object ToolButton2: TToolButton
      Left = 23
      Top = 2
      Caption = 'ToolButton2'
      ImageIndex = 1
    end
    object ToolButton3: TToolButton
      Left = 46
      Top = 2
      Caption = 'ToolButton3'
      ImageIndex = 2
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 572
    Width = 396
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object StringGrid: TStringGrid
    Left = 0
    Top = 29
    Width = 396
    Height = 543
    Align = alClient
    FixedCols = 0
    PopupMenu = PopupMenu
    TabOrder = 2
    ColWidths = (
      19
      78
      74
      73
      64)
  end
  object MainMenu: TMainMenu
    Left = 360
    Top = 8
    object File1: TMenuItem
      Caption = 'File'
      object Open1: TMenuItem
        Caption = '&Open...'
      end
      object Save1: TMenuItem
        Caption = '&Save...'
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
      end
    end
    object About1: TMenuItem
      Caption = 'Help'
      object About2: TMenuItem
        Caption = 'About...'
      end
    end
  end
  object PopupMenu: TPopupMenu
    Left = 360
    Top = 40
    object AddRecord1: TMenuItem
      Caption = '&Add Record...'
    end
    object InsertRecord1: TMenuItem
      Caption = '&Insert Record...'
    end
    object RemoveRecord1: TMenuItem
      Caption = '&Remove Record'
    end
    object Clear1: TMenuItem
      Caption = 'Cl&ear'
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Save2: TMenuItem
      Caption = '&Save'
    end
    object SaveAs1: TMenuItem
      Caption = 'Sa&ve As...'
    end
    object Load1: TMenuItem
      Caption = '&Load...'
    end
  end
end
