<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Public Class MainForm
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing AndAlso components IsNot Nothing Then
            components.Dispose()
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer
    Private mainMenu1 As System.Windows.Forms.MainMenu

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.mainMenu1 = New System.Windows.Forms.MainMenu
        Me.progressBar = New System.Windows.Forms.ProgressBar
        Me.sendBtn = New System.Windows.Forms.Button
        Me.discoverBtn = New System.Windows.Forms.Button
        Me.label1 = New System.Windows.Forms.Label
        Me.deviceList = New System.Windows.Forms.ListBox
        Me.informLabel = New System.Windows.Forms.Label
        Me.statusBar = New System.Windows.Forms.StatusBar
        Me.openFileDialog = New System.Windows.Forms.OpenFileDialog
        Me.SuspendLayout()
        '
        'progressBar
        '
        Me.progressBar.Location = New System.Drawing.Point(16, 140)
        Me.progressBar.Name = "progressBar"
        Me.progressBar.Size = New System.Drawing.Size(208, 8)
        '
        'sendBtn
        '
        Me.sendBtn.Enabled = False
        Me.sendBtn.Location = New System.Drawing.Point(128, 156)
        Me.sendBtn.Name = "sendBtn"
        Me.sendBtn.Size = New System.Drawing.Size(72, 20)
        Me.sendBtn.TabIndex = 8
        Me.sendBtn.Text = "Push"
        '
        'discoverBtn
        '
        Me.discoverBtn.Location = New System.Drawing.Point(32, 156)
        Me.discoverBtn.Name = "discoverBtn"
        Me.discoverBtn.Size = New System.Drawing.Size(72, 20)
        Me.discoverBtn.TabIndex = 9
        Me.discoverBtn.Text = "Discover"
        '
        'label1
        '
        Me.label1.Location = New System.Drawing.Point(14, 4)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(100, 16)
        Me.label1.Text = "Devices"
        '
        'deviceList
        '
        Me.deviceList.Location = New System.Drawing.Point(14, 28)
        Me.deviceList.Name = "deviceList"
        Me.deviceList.Size = New System.Drawing.Size(210, 100)
        Me.deviceList.TabIndex = 11
        '
        'informLabel
        '
        Me.informLabel.Location = New System.Drawing.Point(32, 180)
        Me.informLabel.Name = "informLabel"
        Me.informLabel.Size = New System.Drawing.Size(168, 48)
        Me.informLabel.Text = "Click Discover to find nearby Bluetooth devices."
        '
        'statusBar
        '
        Me.statusBar.Location = New System.Drawing.Point(0, 246)
        Me.statusBar.Name = "statusBar"
        Me.statusBar.Size = New System.Drawing.Size(240, 22)
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(96.0!, 96.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi
        Me.AutoScroll = True
        Me.ClientSize = New System.Drawing.Size(240, 268)
        Me.Controls.Add(Me.progressBar)
        Me.Controls.Add(Me.sendBtn)
        Me.Controls.Add(Me.discoverBtn)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.deviceList)
        Me.Controls.Add(Me.informLabel)
        Me.Controls.Add(Me.statusBar)
        Me.Menu = Me.mainMenu1
        Me.MinimizeBox = False
        Me.Name = "MainForm"
        Me.Text = "Form1"
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents progressBar As System.Windows.Forms.ProgressBar
    Friend WithEvents sendBtn As System.Windows.Forms.Button
    Friend WithEvents discoverBtn As System.Windows.Forms.Button
    Friend WithEvents label1 As System.Windows.Forms.Label
    Friend WithEvents deviceList As System.Windows.Forms.ListBox
    Friend WithEvents informLabel As System.Windows.Forms.Label
    Friend WithEvents statusBar As System.Windows.Forms.StatusBar
    Friend WithEvents openFileDialog As System.Windows.Forms.OpenFileDialog

End Class
