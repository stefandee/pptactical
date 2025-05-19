object FormSprSetup: TFormSprSetup
  Left = 318
  Top = 374
  Width = 201
  Height = 157
  BorderIcons = []
  Caption = 'Sprite Setup'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 24
    Width = 27
    Height = 13
    Caption = 'Rows'
  end
  object Label2: TLabel
    Left = 8
    Top = 56
    Width = 43
    Height = 13
    Caption = 'Columns:'
  end
  object edRows: TCSpinEdit
    Left = 64
    Top = 16
    Width = 121
    Height = 22
    MaxValue = 128
    MinValue = 1
    TabOrder = 0
    Value = 1
  end
  object edCols: TCSpinEdit
    Left = 64
    Top = 48
    Width = 121
    Height = 22
    MaxValue = 128
    MinValue = 1
    TabOrder = 1
    Value = 1
  end
  object btnOk: TButton
    Left = 56
    Top = 96
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 2
  end
end
