object FormMissionInfo: TFormMissionInfo
  Left = 258
  Top = 169
  BorderStyle = bsSingle
  Caption = 'Mission Info'
  ClientHeight = 295
  ClientWidth = 385
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 385
    Height = 57
    Align = alTop
    Caption = 'Mission Name'
    TabOrder = 0
    object edName: TEdit
      Left = 8
      Top = 24
      Width = 369
      Height = 21
      MaxLength = 64
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 57
    Width = 385
    Height = 200
    Align = alTop
    Caption = 'Mission Description'
    TabOrder = 1
    object memoDesc: TMemo
      Left = 8
      Top = 16
      Width = 369
      Height = 177
      Lines.Strings = (
        '')
      MaxLength = 512
      TabOrder = 0
    end
  end
  object btnOk: TButton
    Left = 224
    Top = 264
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 304
    Top = 264
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
