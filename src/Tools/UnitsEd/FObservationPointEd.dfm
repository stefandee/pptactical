object FormObservationPointEditor: TFormObservationPointEditor
  Left = 327
  Top = 317
  BorderStyle = bsSingle
  Caption = 'Observation Point Editor'
  ClientHeight = 196
  ClientWidth = 174
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 40
    Width = 83
    Height = 13
    Caption = 'Min Scan Range:'
  end
  object Label3: TLabel
    Left = 8
    Top = 64
    Width = 86
    Height = 13
    Caption = 'Max Scan Range:'
  end
  object Label4: TLabel
    Left = 8
    Top = 88
    Width = 24
    Height = 13
    Caption = 'FOV:'
  end
  object Label5: TLabel
    Left = 8
    Top = 112
    Width = 58
    Height = 13
    Caption = 'Scan Angle:'
  end
  object edName: TLabeledEdit
    Left = 44
    Top = 8
    Width = 125
    Height = 21
    EditLabel.Width = 34
    EditLabel.Height = 21
    EditLabel.Caption = 'Name: '
    LabelPosition = lpLeft
    TabOrder = 0
    Text = ''
  end
  object edMinScanRange: TCSpinEdit
    Left = 120
    Top = 32
    Width = 49
    Height = 22
    TabOrder = 1
  end
  object edMaxScanRange: TCSpinEdit
    Left = 120
    Top = 56
    Width = 49
    Height = 22
    TabOrder = 2
  end
  object edFOV: TCSpinEdit
    Left = 120
    Top = 80
    Width = 49
    Height = 22
    MaxValue = 359
    TabOrder = 3
  end
  object edScanAngle: TCSpinEdit
    Left = 120
    Top = 104
    Width = 49
    Height = 22
    MaxValue = 359
    TabOrder = 4
  end
  object Button1: TButton
    Left = 10
    Top = 168
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 5
    OnClick = Button1Click
  end
  object cbActive: TCheckBox
    Left = 8
    Top = 136
    Width = 97
    Height = 17
    Caption = 'Active'
    TabOrder = 6
  end
  object Button2: TButton
    Left = 94
    Top = 168
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 7
  end
end
