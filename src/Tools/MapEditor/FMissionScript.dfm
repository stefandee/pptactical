object FormMissionScript: TFormMissionScript
  Left = 796
  Top = 350
  Caption = 'Mission Script'
  ClientHeight = 560
  ClientWidth = 441
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Position = poDesktopCenter
  OnShow = FormShow
  DesignSize = (
    441
    560)
  TextHeight = 13
  object gbScript: TGroupBox
    Left = 0
    Top = 0
    Width = 441
    Height = 387
    Align = alTop
    Caption = 'Script'
    TabOrder = 0
    ExplicitWidth = 449
    DesignSize = (
      441
      387)
    object memoSrc: TMemo
      Left = 2
      Top = 15
      Width = 437
      Height = 330
      Align = alTop
      Lines.Strings = (
        '')
      ScrollBars = ssBoth
      TabOrder = 0
    end
    object btnCompile: TButton
      Left = 366
      Top = 352
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '&Compile'
      TabOrder = 1
      OnClick = btnCompileClick
    end
  end
  object gbErrors: TGroupBox
    Left = 0
    Top = 387
    Width = 441
    Height = 144
    Align = alTop
    Caption = 'Compile Errors'
    TabOrder = 1
    ExplicitWidth = 449
    object memoErrors: TMemo
      Left = 2
      Top = 15
      Width = 437
      Height = 127
      Align = alClient
      Lines.Strings = (
        'Use right click to bring up the popup menu in the memo above.')
      ScrollBars = ssBoth
      TabOrder = 0
    end
  end
  object btnOk: TButton
    Left = 293
    Top = 544
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOkClick
  end
  object Button2: TButton
    Left = 373
    Top = 544
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
