object FormEcoSysGenRandomConfig: TFormEcoSysGenRandomConfig
  Left = 295
  Top = 259
  BorderStyle = bsSingle
  Caption = 'EcoSysGen Random Config'
  ClientHeight = 220
  ClientWidth = 588
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OnShow = FormShow
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 588
    Height = 185
    ActivePage = TabSheet1
    Align = alTop
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Items'
      object DrawGrid: TDrawGrid
        Left = 0
        Top = 0
        Width = 580
        Height = 153
        Align = alTop
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        PopupMenu = PopupMenu1
        TabOrder = 0
        OnDblClick = DrawGridDblClick
        OnDrawCell = DrawGridDrawCell
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Params'
      ImageIndex = 1
      object Label1: TLabel
        Left = 8
        Top = 40
        Width = 30
        Height = 13
        Caption = 'Steps:'
      end
      object Label2: TLabel
        Left = 344
        Top = 8
        Width = 28
        Height = 13
        Caption = 'Zone:'
      end
      object paintZone: TPaintBox
        Left = 344
        Top = 24
        Width = 105
        Height = 105
        PopupMenu = PopupMenu2
        OnMouseDown = paintZoneMouseDown
        OnMouseMove = paintZoneMouseMove
        OnMouseUp = paintZoneMouseUp
        OnPaint = paintZonePaint
      end
      object cbRepeat: TCheckBox
        Left = 8
        Top = 8
        Width = 145
        Height = 17
        Caption = 'Repeat Gen On Collision'
        TabOrder = 0
      end
      object edSteps: TCSpinEdit
        Left = 48
        Top = 32
        Width = 97
        Height = 22
        TabOrder = 1
      end
    end
  end
  object btnOk: TButton
    Left = 208
    Top = 192
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 1
    OnClick = btnOkClick
  end
  object Button2: TButton
    Left = 288
    Top = 192
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object PopupMenu1: TPopupMenu
    Left = 520
    Top = 24
    object Add1: TMenuItem
      Caption = '&Add'
      OnClick = Add1Click
    end
    object Remove1: TMenuItem
      Caption = '&Remove'
      OnClick = Remove1Click
    end
    object Clear1: TMenuItem
      Caption = '&Clear'
      OnClick = Clear1Click
    end
    object Edit1: TMenuItem
      Caption = 'Color Code...'
      OnClick = Edit1Click
    end
  end
  object PopupMenu2: TPopupMenu
    Left = 552
    Top = 24
    object SelectAll1: TMenuItem
      Caption = '&Select All'
      OnClick = SelectAll1Click
    end
    object LeftTopQuarter1: TMenuItem
      Caption = '&Left-Top Quarter'
      OnClick = LeftTopQuarter1Click
    end
    object LeftBottomQuarter1: TMenuItem
      Caption = 'Left-Bottom Quarter'
      OnClick = LeftBottomQuarter1Click
    end
    object RightTopQuarter1: TMenuItem
      Caption = 'Right-Top Quarter'
      OnClick = RightTopQuarter1Click
    end
    object RightBottomQuarter1: TMenuItem
      Caption = 'Right-Bottom Quarter'
      OnClick = RightBottomQuarter1Click
    end
  end
  object ColorDialog: TColorDialog
    Left = 488
    Top = 24
  end
  object OpenDialog: TOpenDialog
    DefaultExt = 'dat'
    Filter = 'DAT|*.dat|All Files|*.*'
    Left = 456
    Top = 24
  end
end
