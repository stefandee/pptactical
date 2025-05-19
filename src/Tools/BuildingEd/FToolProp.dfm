object FormToolProp: TFormToolProp
  Left = 364
  Top = 360
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Properties'
  ClientHeight = 263
  ClientWidth = 357
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OnShow = FormShow
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 357
    Height = 263
    ActivePage = TabMask
    Align = alClient
    TabOrder = 0
    ExplicitWidth = 349
    ExplicitHeight = 251
    object TabMask: TTabSheet
      Caption = 'Mask'
      object Label1: TLabel
        Left = 8
        Top = 8
        Width = 51
        Height = 13
        Caption = 'Brush size:'
      end
      object Label7: TLabel
        Left = 8
        Top = 56
        Width = 53
        Height = 13
        Caption = 'Mask Color'
      end
      object Label8: TLabel
        Left = 8
        Top = 103
        Width = 65
        Height = 13
        Caption = 'Mask Opacity'
      end
      object trackMaskSize: TTrackBar
        Left = 0
        Top = 24
        Width = 150
        Height = 26
        Max = 5
        Min = 1
        Position = 1
        TabOrder = 0
      end
      object MaskColor: TColorBox
        Left = 8
        Top = 75
        Width = 145
        Height = 22
        Selected = clRed
        TabOrder = 1
        OnChange = MaskColorChange
      end
      object trackMaskOpacity: TTrackBar
        Left = 3
        Top = 122
        Width = 150
        Height = 31
        Max = 255
        Frequency = 16
        Position = 128
        TabOrder = 2
        OnChange = trackMaskOpacityChange
      end
    end
    object TabAlt: TTabSheet
      Caption = 'Altitude'
      ImageIndex = 1
      object Label2: TLabel
        Left = 8
        Top = 8
        Width = 38
        Height = 13
        Caption = 'Altitude:'
      end
      object Label3: TLabel
        Left = 8
        Top = 56
        Width = 51
        Height = 13
        Caption = 'Brush size:'
      end
      object editAlt: TCSpinEdit
        Left = 8
        Top = 24
        Width = 121
        Height = 22
        MaxValue = 32768
        MinValue = 1
        TabOrder = 0
      end
      object trackAltSize: TTrackBar
        Left = 0
        Top = 72
        Width = 150
        Height = 45
        Max = 5
        Min = 1
        Position = 1
        TabOrder = 1
      end
    end
    object TabLayers: TTabSheet
      Caption = 'Layers'
      ImageIndex = 2
      object Label4: TLabel
        Left = 0
        Top = 184
        Width = 84
        Height = 13
        Caption = 'Frame Delay (ms):'
      end
      object btnUp: TSpeedButton
        Left = 296
        Top = 160
        Width = 23
        Height = 22
        OnClick = btnUpClick
      end
      object btnLeft: TSpeedButton
        Left = 272
        Top = 184
        Width = 23
        Height = 22
        OnClick = btnLeftClick
      end
      object btnRight: TSpeedButton
        Left = 320
        Top = 184
        Width = 23
        Height = 22
        OnClick = btnRightClick
      end
      object btnDown: TSpeedButton
        Left = 296
        Top = 208
        Width = 23
        Height = 22
        OnClick = btnDownClick
      end
      object Label5: TLabel
        Left = 216
        Top = 160
        Width = 25
        Height = 13
        Caption = 'Step:'
      end
      object Label6: TLabel
        Left = 88
        Top = 184
        Width = 81
        Height = 13
        Caption = 'Cycle Delay (ms):'
      end
      object gridLayers: TStringGrid
        Left = 0
        Top = 0
        Width = 349
        Height = 153
        Align = alTop
        ColCount = 2
        FixedCols = 0
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
        PopupMenu = PopupMenu1
        ScrollBars = ssVertical
        TabOrder = 0
        OnClick = gridLayersClick
        ColWidths = (
          151
          145)
      end
      object cbVisible: TCheckBox
        Left = 0
        Top = 160
        Width = 97
        Height = 17
        Caption = 'Visible'
        TabOrder = 1
        OnClick = cbVisibleClick
      end
      object edFrameDelay: TCSpinEdit
        Left = 0
        Top = 200
        Width = 65
        Height = 22
        MaxValue = 100000
        TabOrder = 2
        Value = 1
        OnChange = edFrameDelayChange
      end
      object edStep: TCSpinEdit
        Left = 216
        Top = 176
        Width = 49
        Height = 22
        MaxValue = 100
        MinValue = 1
        TabOrder = 3
        Value = 1
      end
      object edCycleDelay: TCSpinEdit
        Left = 88
        Top = 200
        Width = 65
        Height = 22
        MaxValue = 100000
        TabOrder = 4
        Value = 1
        OnChange = edCycleDelayChange
      end
    end
    object TabPoints: TTabSheet
      Caption = 'Points'
      ImageIndex = 3
      object btnEditVisual: TSpeedButton
        Left = 190
        Top = 50
        Width = 23
        Height = 22
        Caption = 'ed'
        Spacing = 0
        OnClick = btnEditVisualClick
      end
      object btnAddVisual: TSpeedButton
        Left = 216
        Top = 50
        Width = 23
        Height = 22
        Caption = 'add'
        Spacing = 0
        OnClick = btnAddVisualClick
      end
      object btnRemoveVisual: TSpeedButton
        Left = 244
        Top = 50
        Width = 23
        Height = 22
        Caption = 'rm'
        Spacing = 0
        OnClick = btnRemoveVisualClick
      end
      object rbDropPoint: TRadioButton
        Left = 8
        Top = 8
        Width = 113
        Height = 17
        Caption = 'Drop Point'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = rbDropPointClick
      end
      object rbFirePoint: TRadioButton
        Left = 8
        Top = 32
        Width = 113
        Height = 17
        Caption = 'Fire Point'
        TabOrder = 1
        OnClick = rbDropPointClick
      end
      object rbVisualPoint: TRadioButton
        Left = 8
        Top = 56
        Width = 113
        Height = 17
        Caption = 'Visual'
        TabOrder = 2
        OnClick = rbDropPointClick
      end
      object cbVisual: TComboBox
        Left = 88
        Top = 52
        Width = 97
        Height = 21
        Style = csDropDownList
        TabOrder = 3
        OnChange = cbVisualChange
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 320
    Top = 144
    object NewLayer1: TMenuItem
      Caption = '&New Layer'
      OnClick = NewLayer1Click
    end
    object DeleteLayer1: TMenuItem
      Caption = '&Delete Layer'
      OnClick = DeleteLayer1Click
    end
    object RenameLayer1: TMenuItem
      Caption = '&Rename Layer...'
    end
    object Sprite1: TMenuItem
      Caption = '&Sprite...'
      OnClick = Sprite1Click
    end
    object SpriteSetup1: TMenuItem
      Caption = 'Sprite Setup...'
      OnClick = SpriteSetup1Click
    end
  end
  object OpenDialog: TOpenDialog
    Left = 288
    Top = 144
  end
end
