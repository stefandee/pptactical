object FormBayManager: TFormBayManager
  Left = 589
  Top = 622
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Bay Manager'
  ClientHeight = 346
  ClientWidth = 679
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OnDestroy = FormDestroy
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 297
    Height = 327
    Align = alLeft
    Caption = 'In Bay'
    TabOrder = 0
    ExplicitHeight = 328
    object gridBay: TStringGrid
      Left = 2
      Top = 15
      Width = 293
      Height = 311
      Align = alClient
      ColCount = 4
      DefaultRowHeight = 16
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      ScrollBars = ssVertical
      TabOrder = 0
      ColWidths = (
        17
        120
        83
        64)
    end
  end
  object GroupBox2: TGroupBox
    Left = 381
    Top = 0
    Width = 298
    Height = 327
    Align = alRight
    Caption = 'Available Units'
    TabOrder = 1
    ExplicitLeft = 385
    ExplicitHeight = 328
    object gridAvail: TStringGrid
      Left = 2
      Top = 15
      Width = 294
      Height = 311
      Align = alClient
      ColCount = 4
      DefaultRowHeight = 16
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      ScrollBars = ssVertical
      TabOrder = 0
      ColWidths = (
        17
        120
        84
        64)
    end
  end
  object btnAdd: TButton
    Left = 304
    Top = 104
    Width = 75
    Height = 25
    Caption = '<Add'
    TabOrder = 2
    OnClick = btnAddClick
  end
  object btnRemove: TButton
    Left = 304
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Remove>'
    TabOrder = 3
    OnClick = btnRemoveClick
  end
  object btnClearBay: TButton
    Left = 304
    Top = 168
    Width = 75
    Height = 25
    Caption = 'Clear Bay'
    TabOrder = 4
    OnClick = btnClearBayClick
  end
  object statusBay: TStatusBar
    Left = 0
    Top = 327
    Width = 679
    Height = 19
    Panels = <
      item
        Width = 50
      end>
    SimplePanel = True
    ExplicitTop = 328
    ExplicitWidth = 683
  end
  object Button4: TButton
    Left = 304
    Top = 296
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 6
  end
end
