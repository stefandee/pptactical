object FormMapGen: TFormMapGen
  Left = 434
  Top = 428
  AlphaBlendValue = 128
  Caption = 'Map Generator'
  ClientHeight = 448
  ClientWidth = 552
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OnShow = FormShow
  DesignSize = (
    552
    448)
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 552
    Height = 401
    Align = alTop
    Caption = 'Options'
    TabOrder = 0
    ExplicitWidth = 560
    object pcGenOptions: TPageControl
      Left = 2
      Top = 15
      Width = 548
      Height = 384
      ActivePage = TabSheet3
      Align = alClient
      HotTrack = True
      MultiLine = True
      TabOrder = 0
      ExplicitWidth = 556
      object TabSheet1: TTabSheet
        Caption = 'Tiles'
        object imgPreview: TImage
          Left = 348
          Top = 80
          Width = 96
          Height = 96
        end
        object Label1: TLabel
          Left = 16
          Top = 12
          Width = 25
          Height = 13
          Caption = '&Tiles:'
          FocusControl = listTile
        end
        object lbRate: TLabel
          Left = 212
          Top = 12
          Width = 82
          Height = 13
          Caption = 'A&ppearance rate:'
          FocusControl = listRate
        end
        object lbPreview: TLabel
          Left = 404
          Top = 60
          Width = 38
          Height = 13
          Caption = 'Preview'
        end
        object lbSetRate: TLabel
          Left = 344
          Top = 12
          Width = 40
          Height = 13
          Caption = '&Set rate:'
          FocusControl = comboRate
        end
        object listTile: TListBox
          Left = 12
          Top = 32
          Width = 197
          Height = 144
          Style = lbOwnerDrawFixed
          IntegralHeight = True
          ItemHeight = 20
          TabOrder = 0
          OnClick = listTileClick
        end
        object listRate: TListBox
          Left = 212
          Top = 32
          Width = 125
          Height = 144
          Style = lbOwnerDrawFixed
          IntegralHeight = True
          ItemHeight = 20
          TabOrder = 1
          OnClick = listRateClick
        end
        object comboRate: TComboBox
          Left = 340
          Top = 32
          Width = 105
          Height = 21
          Style = csDropDownList
          Enabled = False
          TabOrder = 2
          OnChange = comboRateChange
          Items.Strings = (
            'very seldom'
            'seldom'
            'normal'
            'often'
            'very often')
        end
        object btnAdd: TBitBtn
          Left = 12
          Top = 184
          Width = 50
          Height = 25
          Caption = '&Add'
          TabOrder = 3
          OnClick = btnAddClick
        end
        object btnDelete: TBitBtn
          Left = 72
          Top = 184
          Width = 50
          Height = 25
          Caption = '&Delete'
          TabOrder = 4
          OnClick = btnDeleteClick
        end
        object btnClear: TBitBtn
          Left = 132
          Top = 184
          Width = 50
          Height = 25
          Caption = 'Cl&ear'
          TabOrder = 5
          OnClick = btnClearClick
        end
      end
      object TabSheet2: TTabSheet
        Caption = 'Altitude and Edges'
        ImageIndex = 1
        object paintHeight: TPaintBox
          Left = 304
          Top = 0
          Width = 241
          Height = 249
        end
        object Label2: TLabel
          Left = 16
          Top = 16
          Width = 84
          Height = 13
          Caption = 'Altitude Algorithm:'
        end
        object Label3: TLabel
          Left = 16
          Top = 80
          Width = 74
          Height = 13
          Caption = 'Edge Algorithm:'
        end
        object btnHMPreview: TButton
          Left = 472
          Top = 256
          Width = 75
          Height = 25
          Caption = 'Preview'
          TabOrder = 0
        end
        object ComboBox1: TComboBox
          Left = 128
          Top = 8
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 1
          Text = 'Fault'
          Items.Strings = (
            'Fault'
            'Fluid'
            'Hill'
            'Scripted')
        end
        object ComboBox2: TComboBox
          Left = 128
          Top = 72
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 2
          Text = 'Basic'
          Items.Strings = (
            'Basic')
        end
        object Button1: TButton
          Left = 200
          Top = 32
          Width = 75
          Height = 25
          Caption = '&Configure'
          TabOrder = 3
        end
        object Button2: TButton
          Left = 200
          Top = 96
          Width = 75
          Height = 25
          Caption = '&Configure'
          TabOrder = 4
        end
      end
      object TabSheet3: TTabSheet
        Caption = 'EcoSystem'
        ImageIndex = 2
        object Label4: TLabel
          Left = 16
          Top = 16
          Width = 102
          Height = 13
          Caption = 'EcoSystem Algorithm:'
        end
        object paintEcoSys: TPaintBox
          Left = 304
          Top = 0
          Width = 241
          Height = 249
        end
        object cbEcoSysGenAlg: TComboBox
          Left = 128
          Top = 8
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 0
          Text = 'Random'
          OnChange = cbEcoSysGenAlgChange
          Items.Strings = (
            'Random'
            'Probability Fields')
        end
        object btnEcoSysConfig: TButton
          Left = 200
          Top = 32
          Width = 75
          Height = 25
          Caption = '&Configure'
          TabOrder = 1
          OnClick = btnEcoSysConfigClick
        end
        object btnEcoSysPreview: TButton
          Left = 472
          Top = 256
          Width = 75
          Height = 25
          Caption = 'Preview'
          TabOrder = 2
          OnClick = btnEcoSysPreviewClick
        end
        object btnEcoSysGen: TButton
          Left = 392
          Top = 256
          Width = 75
          Height = 25
          Caption = 'Generate'
          TabOrder = 3
          OnClick = btnEcoSysGenClick
        end
      end
    end
  end
  object btnCancelGenerate: TButton
    Left = 480
    Top = 407
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object btnOkGenerate: TButton
    Left = 400
    Top = 407
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOkGenerateClick
  end
  object statusMain: TStatusBar
    Left = 0
    Top = 429
    Width = 552
    Height = 19
    Panels = <
      item
        Width = 50
      end>
    ExplicitTop = 441
    ExplicitWidth = 560
  end
  object dialogOpen: TOpenDialog
    DefaultExt = 'dat'
    Filter = 'Tile file|*.dat|All files (*.*)|*.*'
    Title = 'Open Tile File'
    Left = 36
    Top = 368
  end
  object Timer: TTimer
    Interval = 500
    OnTimer = TimerTimer
    Left = 4
    Top = 368
  end
end
