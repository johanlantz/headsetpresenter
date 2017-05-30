Public Class InputForm
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents okBtn As System.Windows.Forms.Button
    Friend WithEvents cancelBtn As System.Windows.Forms.Button
    Friend WithEvents TextBox As System.Windows.Forms.TextBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.Label1 = New System.Windows.Forms.Label
        Me.TextBox = New System.Windows.Forms.TextBox
        Me.okBtn = New System.Windows.Forms.Button
        Me.cancelBtn = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(8, 8)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(176, 23)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Input name of new sub-directory:"
        '
        'TextBox
        '
        Me.TextBox.Location = New System.Drawing.Point(8, 32)
        Me.TextBox.Name = "TextBox"
        Me.TextBox.Size = New System.Drawing.Size(200, 20)
        Me.TextBox.TabIndex = 1
        Me.TextBox.Text = "New folder"
        '
        'okBtn
        '
        Me.okBtn.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.okBtn.Location = New System.Drawing.Point(136, 64)
        Me.okBtn.Name = "okBtn"
        Me.okBtn.TabIndex = 2
        Me.okBtn.Text = "OK"
        '
        'cancelBtn
        '
        Me.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.cancelBtn.Location = New System.Drawing.Point(56, 64)
        Me.cancelBtn.Name = "cancelBtn"
        Me.cancelBtn.TabIndex = 3
        Me.cancelBtn.Text = "Cancel"
        '
        'InputForm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(216, 93)
        Me.Controls.Add(Me.cancelBtn)
        Me.Controls.Add(Me.okBtn)
        Me.Controls.Add(Me.TextBox)
        Me.Controls.Add(Me.Label1)
        Me.Name = "InputForm"
        Me.Text = "InputForm"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Public ReadOnly Property FolderName()
        Get
            Return TextBox.Text
        End Get
    End Property
End Class
