object FormEditCurve2D: TFormEditCurve2D
  Left = 528
  Top = 133
  BorderStyle = bsSingle
  Caption = 'Curve2D Editor'
  ClientHeight = 614
  ClientWidth = 389
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 185
    Height = 73
    Caption = 'General'
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 48
      Width = 27
      Height = 13
      Caption = 'Type:'
    end
    object Label2: TLabel
      Left = 8
      Top = 24
      Width = 31
      Height = 13
      Caption = 'Name:'
    end
    object cbCurveType: TComboBox
      Left = 56
      Top = 40
      Width = 121
      Height = 21
      Style = csDropDownList
      TabOrder = 0
      OnChange = cbCurveTypeChange
      Items.Strings = (
        'Linear'
        'Elliptic'
        'Polynomial'
        'Scripted'
        'Composite')
    end
    object edName: TEdit
      Left = 56
      Top = 16
      Width = 121
      Height = 21
      TabOrder = 1
    end
  end
  object GroupBox2: TGroupBox
    Left = 200
    Top = 8
    Width = 185
    Height = 281
    Caption = 'Parameters'
    TabOrder = 1
    object Label3: TLabel
      Left = 8
      Top = 24
      Width = 27
      Height = 13
      Caption = 'Type:'
    end
    object listValues: TValueListEditor
      Left = 2
      Top = 47
      Width = 181
      Height = 226
      KeyOptions = [keyEdit, keyAdd, keyDelete, keyUnique]
      TabOrder = 0
      TitleCaptions.Strings = (
        'Parameter'
        'Value')
      ColWidths = (
        95
        80)
    end
    object cbParamType: TComboBox
      Left = 64
      Top = 16
      Width = 113
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 1
      Text = 'Parametric'
      Items.Strings = (
        'Parametric'
        'Implicit'
        'Explicit')
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 88
    Width = 185
    Height = 201
    Caption = 'Variables'
    TabOrder = 2
    object Label4: TLabel
      Left = 112
      Top = 24
      Width = 9
      Height = 13
      Caption = 'to'
    end
    object Label5: TLabel
      Left = 8
      Top = 24
      Width = 38
      Height = 13
      Caption = 'Interval:'
    end
    object labelScript: TLabel
      Left = 8
      Top = 48
      Width = 27
      Height = 13
      Caption = 'Script'
      Visible = False
    end
    object edIntervalMin: TEdit
      Left = 56
      Top = 16
      Width = 49
      Height = 21
      TabOrder = 0
      Text = '-100'
    end
    object edIntervalMax: TEdit
      Left = 128
      Top = 16
      Width = 49
      Height = 21
      TabOrder = 1
      Text = '100'
    end
    object memoScript: TMemo
      Left = 8
      Top = 64
      Width = 169
      Height = 129
      TabOrder = 2
      Visible = False
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 296
    Width = 377
    Height = 281
    Caption = 'Preview'
    TabOrder = 3
    object paintBox: TPaintBox
      Left = 168
      Top = 16
      Width = 200
      Height = 200
      Color = clBlack
      ParentColor = False
      OnMouseDown = paintBoxMouseDown
      OnMouseMove = paintBoxMouseMove
      OnMouseUp = paintBoxMouseUp
      OnPaint = paintBoxPaint
    end
    object SpeedButton1: TSpeedButton
      Left = 320
      Top = 224
      Width = 23
      Height = 22
      Hint = 'Generate Preview'
      Caption = 'G'
      OnClick = SpeedButton1Click
    end
    object SpeedButton2: TSpeedButton
      Left = 344
      Top = 224
      Width = 23
      Height = 22
      Hint = 'Center in Origin'
      Caption = 'C'
      OnClick = SpeedButton2Click
    end
    object Label6: TLabel
      Left = 8
      Top = 24
      Width = 38
      Height = 13
      Caption = 'Interval:'
    end
    object Label7: TLabel
      Left = 64
      Top = 48
      Width = 9
      Height = 13
      Caption = 'to'
    end
    object Label8: TLabel
      Left = 8
      Top = 72
      Width = 25
      Height = 13
      Caption = 'Step:'
    end
    object shapeCurve: TShape
      Left = 8
      Top = 248
      Width = 24
      Height = 24
      OnMouseUp = shapeCurveMouseUp
    end
    object shapeAxes: TShape
      Left = 56
      Top = 248
      Width = 24
      Height = 24
      OnMouseUp = shapeAxesMouseUp
    end
    object shapeBack: TShape
      Left = 104
      Top = 248
      Width = 24
      Height = 24
      OnMouseUp = shapeBackMouseUp
    end
    object Label9: TLabel
      Left = 8
      Top = 232
      Width = 31
      Height = 13
      Caption = 'Curve:'
    end
    object Label10: TLabel
      Left = 56
      Top = 232
      Width = 26
      Height = 13
      Caption = 'Axes:'
    end
    object Label11: TLabel
      Left = 104
      Top = 232
      Width = 25
      Height = 13
      Caption = 'Back'
    end
    object shapeMarker: TShape
      Left = 152
      Top = 248
      Width = 24
      Height = 24
      OnMouseUp = shapeMarkerMouseUp
    end
    object Label12: TLabel
      Left = 152
      Top = 232
      Width = 33
      Height = 13
      Caption = 'Marker'
    end
    object Label13: TLabel
      Left = 8
      Top = 96
      Width = 83
      Height = 13
      Caption = 'Markers Spacing:'
    end
    object edDrawIntervalMin: TEdit
      Left = 8
      Top = 40
      Width = 49
      Height = 21
      TabOrder = 0
      Text = '-100'
    end
    object edDrawIntervalMax: TEdit
      Left = 80
      Top = 40
      Width = 49
      Height = 21
      TabOrder = 1
      Text = '100'
    end
    object edDrawIntervalStep: TEdit
      Left = 112
      Top = 64
      Width = 49
      Height = 21
      TabOrder = 2
      Text = '1'
    end
    object edSpacing: TEdit
      Left = 112
      Top = 88
      Width = 49
      Height = 21
      TabOrder = 3
      Text = '10'
      OnExit = edSpacingExit
    end
  end
  object Button1: TButton
    Left = 117
    Top = 584
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 4
  end
  object Button2: TButton
    Left = 197
    Top = 584
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 5
  end
  object dlgColor: TColorDialog
    Left = 56
    Top = 160
  end
end
