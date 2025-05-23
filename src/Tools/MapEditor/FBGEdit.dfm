object FormBGEdit: TFormBGEdit
  Left = 271
  Top = 155
  BorderStyle = bsSingle
  Caption = 'Battle Group Management'
  ClientHeight = 542
  ClientWidth = 738
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Position = poDesktopCenter
  OnShow = FormShow
  TextHeight = 13
  object btnOk: TButton
    Left = 664
    Top = 512
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 738
    Height = 497
    Align = alTop
    Caption = 'Battle Group edit area'
    TabOrder = 1
    ExplicitWidth = 742
    object Label3: TLabel
      Left = 8
      Top = 80
      Width = 53
      Height = 13
      Caption = 'Description'
    end
    object btnPrev: TSpeedButton
      Left = 264
      Top = 464
      Width = 23
      Height = 22
      Caption = '<'
      OnClick = btnPrevClick
    end
    object btnNext: TSpeedButton
      Left = 336
      Top = 464
      Width = 23
      Height = 22
      Caption = '>'
      OnClick = btnNextClick
    end
    object lProgress: TLabel
      Left = 288
      Top = 470
      Width = 48
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = '1/2'
    end
    object gbScript: TGroupBox
      Left = 368
      Top = 16
      Width = 369
      Height = 473
      Caption = 'Battle Group Script'
      TabOrder = 0
      object memoSrc: TMemo
        Left = 8
        Top = 16
        Width = 353
        Height = 313
        ScrollBars = ssBoth
        TabOrder = 0
        OnExit = memoSrcExit
      end
      object btnCompile: TButton
        Left = 286
        Top = 336
        Width = 75
        Height = 25
        Caption = 'Compile'
        TabOrder = 1
        OnClick = btnCompileClick
      end
      object memoErrors: TMemo
        Left = 8
        Top = 368
        Width = 353
        Height = 97
        Lines.Strings = (
          'Use right click to bring up the popup menu in the memo above.')
        ReadOnly = True
        ScrollBars = ssBoth
        TabOrder = 2
      end
    end
    object btnDelete: TButton
      Left = 88
      Top = 464
      Width = 75
      Height = 25
      Caption = '&Delete'
      TabOrder = 1
      OnClick = btnDeleteClick
    end
    object btnAdd: TButton
      Left = 8
      Top = 464
      Width = 75
      Height = 25
      Caption = '&New'
      TabOrder = 2
      OnClick = btnAddClick
    end
    object gbData: TGroupBox
      Left = 8
      Top = 16
      Width = 353
      Height = 393
      Caption = 'Data'
      TabOrder = 3
      object Label1: TLabel
        Left = 8
        Top = 32
        Width = 31
        Height = 13
        Caption = 'Name:'
      end
      object Label2: TLabel
        Left = 8
        Top = 56
        Width = 12
        Height = 13
        Caption = 'Id:'
      end
      object edName: TEdit
        Left = 64
        Top = 24
        Width = 281
        Height = 21
        MaxLength = 32
        TabOrder = 0
        OnExit = edNameExit
      end
      object edId: TEdit
        Left = 64
        Top = 48
        Width = 281
        Height = 21
        TabOrder = 1
        OnExit = edIdExit
      end
      object memoDesc: TMemo
        Left = 8
        Top = 96
        Width = 337
        Height = 289
        MaxLength = 512
        ScrollBars = ssBoth
        TabOrder = 2
        OnExit = memoDescExit
      end
    end
  end
end
