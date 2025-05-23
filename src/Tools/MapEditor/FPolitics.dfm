object FormPolitics: TFormPolitics
  Left = 229
  Top = 107
  Width = 696
  Height = 480
  Caption = 'Battle Groups Politics'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  DesignSize = (
    688
    453)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 688
    Height = 417
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Politics'
    TabOrder = 0
    object GroupBox2: TGroupBox
      Left = 2
      Top = 15
      Width = 185
      Height = 400
      Align = alLeft
      Caption = 'Setup'
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 24
        Width = 52
        Height = 13
        Caption = 'Draw style:'
      end
      object Label2: TLabel
        Left = 8
        Top = 72
        Width = 54
        Height = 13
        Caption = 'Show links:'
      end
      object Label3: TLabel
        Left = 8
        Top = 120
        Width = 52
        Height = 13
        Caption = 'Draw links:'
      end
      object cbStyle: TComboBox
        Left = 8
        Top = 40
        Width = 169
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = cbStyleChange
        Items.Strings = (
          'Graph 1'
          'Graph 2')
      end
      object cbLinks: TComboBox
        Left = 8
        Top = 88
        Width = 169
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 1
        OnChange = cbLinksChange
        Items.Strings = (
          'Show all'
          'Show neutral'
          'Show enemy'
          'Show friendly'
          'Show for selected')
      end
      object cbDraw: TComboBox
        Left = 8
        Top = 136
        Width = 169
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 2
        OnChange = cbDrawChange
        Items.Strings = (
          'Neutral'
          'Enemy'
          'Friendly')
      end
      object cboxNames: TCheckBox
        Left = 8
        Top = 168
        Width = 97
        Height = 17
        Caption = 'Show Names'
        Checked = True
        State = cbChecked
        TabOrder = 3
        OnClick = cboxNamesClick
      end
      object cboxId: TCheckBox
        Left = 8
        Top = 192
        Width = 97
        Height = 17
        Caption = 'Show Id'
        TabOrder = 4
        OnClick = cboxIdClick
      end
    end
    object GroupBox3: TGroupBox
      Left = 187
      Top = 15
      Width = 499
      Height = 400
      Align = alClient
      Caption = 'Visual'
      TabOrder = 1
      object pbScreen: TPaintBox
        Left = 2
        Top = 15
        Width = 495
        Height = 383
        Align = alClient
        OnMouseDown = pbScreenMouseDown
        OnMouseMove = pbScreenMouseMove
        OnMouseUp = pbScreenMouseUp
        OnPaint = pbScreenPaint
      end
    end
  end
  object btnOk: TButton
    Left = 608
    Top = 424
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 1
  end
end
