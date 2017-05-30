<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class MainForm
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

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.statusBar = New System.Windows.Forms.StatusBar
        Me.statusBarPanel = New System.Windows.Forms.StatusBarPanel
        Me.copyPanel = New System.Windows.Forms.StatusBarPanel
        Me.progressBar = New System.Windows.Forms.ProgressBar
        Me.sendBtn = New System.Windows.Forms.Button
        Me.discoverBtn = New System.Windows.Forms.Button
        Me.label1 = New System.Windows.Forms.Label
        Me.deviceList = New System.Windows.Forms.ListBox
        Me.openFileDialog = New System.Windows.Forms.OpenFileDialog
        Me.informLabel = New System.Windows.Forms.Label
        CType(Me.statusBarPanel, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.copyPanel, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'statusBar
        '
        Me.statusBar.Location = New System.Drawing.Point(0, 293)
        Me.statusBar.Name = "statusBar"
        Me.statusBar.Panels.AddRange(New System.Windows.Forms.StatusBarPanel() {Me.statusBarPanel, Me.copyPanel})
        Me.statusBar.ShowPanels = True
        Me.statusBar.Size = New System.Drawing.Size(190, 22)
        Me.statusBar.SizingGrip = False
        Me.statusBar.TabIndex = 19
        '
        'statusBarPanel
        '
        Me.statusBarPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring
        Me.statusBarPanel.Name = "statusBarPanel"
        Me.statusBarPanel.Width = 180
        '
        'copyPanel
        '
        Me.copyPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents
        Me.copyPanel.Name = "copyPanel"
        Me.copyPanel.Width = 10
        '
        'progressBar
        '
        Me.progressBar.Location = New System.Drawing.Point(16, 194)
        Me.progressBar.Name = "progressBar"
        Me.progressBar.Size = New System.Drawing.Size(160, 8)
        Me.progressBar.TabIndex = 18
        '
        'sendBtn
        '
        Me.sendBtn.Enabled = False
        Me.sendBtn.Location = New System.Drawing.Point(104, 210)
        Me.sendBtn.Name = "sendBtn"
        Me.sendBtn.Size = New System.Drawing.Size(75, 23)
        Me.sendBtn.TabIndex = 17
        Me.sendBtn.Text = "Push"
        '
        'discoverBtn
        '
        Me.discoverBtn.Location = New System.Drawing.Point(16, 210)
        Me.discoverBtn.Name = "discoverBtn"
        Me.discoverBtn.Size = New System.Drawing.Size(75, 23)
        Me.discoverBtn.TabIndex = 16
        Me.discoverBtn.Text = "Discover"
        '
        'label1
        '
        Me.label1.Location = New System.Drawing.Point(16, 2)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(100, 16)
        Me.label1.TabIndex = 15
        Me.label1.Text = "Devices"
        '
        'deviceList
        '
        Me.deviceList.Location = New System.Drawing.Point(16, 26)
        Me.deviceList.Name = "deviceList"
        Me.deviceList.Size = New System.Drawing.Size(160, 160)
        Me.deviceList.TabIndex = 14
        '
        'openFileDialog
        '
        Me.openFileDialog.Multiselect = True
        '
        'informLabel
        '
        Me.informLabel.Location = New System.Drawing.Point(16, 242)
        Me.informLabel.Name = "informLabel"
        Me.informLabel.Size = New System.Drawing.Size(160, 40)
        Me.informLabel.TabIndex = 20
        Me.informLabel.Text = "Click Discover to find nearby Bluetooth devices."
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(190, 315)
        Me.Controls.Add(Me.statusBar)
        Me.Controls.Add(Me.progressBar)
        Me.Controls.Add(Me.sendBtn)
        Me.Controls.Add(Me.discoverBtn)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.deviceList)
        Me.Controls.Add(Me.informLabel)
        Me.Name = "MainForm"
        Me.Text = "ObjectPush Sample"
        CType(Me.statusBarPanel, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.copyPanel, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents statusBar As System.Windows.Forms.StatusBar
    Friend WithEvents statusBarPanel As System.Windows.Forms.StatusBarPanel
    Friend WithEvents copyPanel As System.Windows.Forms.StatusBarPanel
    Friend WithEvents progressBar As System.Windows.Forms.ProgressBar
    Friend WithEvents sendBtn As System.Windows.Forms.Button
    Friend WithEvents discoverBtn As System.Windows.Forms.Button
    Friend WithEvents label1 As System.Windows.Forms.Label
    Friend WithEvents deviceList As System.Windows.Forms.ListBox
    Friend WithEvents openFileDialog As System.Windows.Forms.OpenFileDialog
    Friend WithEvents informLabel As System.Windows.Forms.Label
End Class
