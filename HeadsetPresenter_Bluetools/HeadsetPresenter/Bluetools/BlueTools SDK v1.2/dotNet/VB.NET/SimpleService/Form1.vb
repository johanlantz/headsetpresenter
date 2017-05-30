Option Explicit On 

Imports Franson.BlueTools

Public Class Form1
    Inherits System.Windows.Forms.Form

    Friend WithEvents bAdvertise As System.Windows.Forms.Button
    Friend WithEvents bDeadvertise As System.Windows.Forms.Button
    Friend WithEvents txtWriteData As System.Windows.Forms.TextBox
    Friend WithEvents bWrite As System.Windows.Forms.Button
    Friend WithEvents txtRead As System.Windows.Forms.TextBox
    Friend WithEvents label1 As System.Windows.Forms.Label
    Friend WithEvents label2 As System.Windows.Forms.Label
    Friend WithEvents lServiceStatus As System.Windows.Forms.Label
    Friend WithEvents lStackID As System.Windows.Forms.Label
    Friend WithEvents listSessions As System.Windows.Forms.ListBox
    Friend WithEvents label3 As System.Windows.Forms.Label
    Friend WithEvents label4 As System.Windows.Forms.Label
    Friend WithEvents label5 As System.Windows.Forms.Label
    Friend WithEvents label8 As System.Windows.Forms.Label
    Friend WithEvents txtRead2 As System.Windows.Forms.TextBox
    Friend WithEvents bWrite2 As System.Windows.Forms.Button
    Friend WithEvents bAdvertise2 As System.Windows.Forms.Button
    Friend WithEvents txtWriteData2 As System.Windows.Forms.TextBox
    Friend WithEvents listSessions2 As System.Windows.Forms.ListBox
    Friend WithEvents lServiceStatus2 As System.Windows.Forms.Label
    Friend WithEvents bDeadvertise2 As System.Windows.Forms.Button
    Friend WithEvents txtSCN As System.Windows.Forms.TextBox
    Friend WithEvents txtSCN2 As System.Windows.Forms.TextBox
    Friend WithEvents cbSerialPort2 As System.Windows.Forms.CheckBox
    Friend WithEvents cbSerialPort As System.Windows.Forms.CheckBox
    Friend WithEvents bCloseSession2 As System.Windows.Forms.Button
    Friend WithEvents bCloseSession1 As System.Windows.Forms.Button

    Friend WithEvents m_network As Network
    Friend WithEvents m_service As LocalService
    Friend WithEvents m_service2 As LocalService
    Dim m_manager As Manager


#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub





    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.bAdvertise = New System.Windows.Forms.Button
        Me.bDeadvertise = New System.Windows.Forms.Button
        Me.txtWriteData = New System.Windows.Forms.TextBox
        Me.bWrite = New System.Windows.Forms.Button
        Me.txtRead = New System.Windows.Forms.TextBox
        Me.label1 = New System.Windows.Forms.Label
        Me.label2 = New System.Windows.Forms.Label
        Me.lServiceStatus = New System.Windows.Forms.Label
        Me.lStackID = New System.Windows.Forms.Label
        Me.listSessions = New System.Windows.Forms.ListBox
        Me.label3 = New System.Windows.Forms.Label
        Me.label4 = New System.Windows.Forms.Label
        Me.label5 = New System.Windows.Forms.Label
        Me.txtRead2 = New System.Windows.Forms.TextBox
        Me.bWrite2 = New System.Windows.Forms.Button
        Me.bAdvertise2 = New System.Windows.Forms.Button
        Me.txtWriteData2 = New System.Windows.Forms.TextBox
        Me.bDeadvertise2 = New System.Windows.Forms.Button
        Me.lServiceStatus2 = New System.Windows.Forms.Label
        Me.listSessions2 = New System.Windows.Forms.ListBox
        Me.label8 = New System.Windows.Forms.Label
        Me.txtSCN = New System.Windows.Forms.TextBox
        Me.txtSCN2 = New System.Windows.Forms.TextBox
        Me.cbSerialPort = New System.Windows.Forms.CheckBox
        Me.cbSerialPort2 = New System.Windows.Forms.CheckBox
        Me.bCloseSession2 = New System.Windows.Forms.Button
        Me.bCloseSession1 = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'bAdvertise
        '
        Me.bAdvertise.Location = New System.Drawing.Point(24, 24)
        Me.bAdvertise.Name = "bAdvertise"
        Me.bAdvertise.Size = New System.Drawing.Size(96, 24)
        Me.bAdvertise.TabIndex = 0
        Me.bAdvertise.Text = "Advertise"
        '
        'bDeadvertise
        '
        Me.bDeadvertise.Location = New System.Drawing.Point(24, 56)
        Me.bDeadvertise.Name = "bDeadvertise"
        Me.bDeadvertise.Size = New System.Drawing.Size(96, 24)
        Me.bDeadvertise.TabIndex = 0
        Me.bDeadvertise.Text = "Deadvertise"
        '
        'txtWriteData
        '
        Me.txtWriteData.Location = New System.Drawing.Point(32, 136)
        Me.txtWriteData.Name = "txtWriteData"
        Me.txtWriteData.Size = New System.Drawing.Size(216, 20)
        Me.txtWriteData.TabIndex = 0
        Me.txtWriteData.Text = ""
        '
        'bWrite
        '
        Me.bWrite.Location = New System.Drawing.Point(256, 136)
        Me.bWrite.Name = "bWrite"
        Me.bWrite.Size = New System.Drawing.Size(56, 24)
        Me.bWrite.TabIndex = 0
        Me.bWrite.Text = "Write"
        '
        'txtRead
        '
        Me.txtRead.Location = New System.Drawing.Point(32, 184)
        Me.txtRead.Name = "txtRead"
        Me.txtRead.Size = New System.Drawing.Size(216, 20)
        Me.txtRead.TabIndex = 0
        Me.txtRead.Text = ""
        '
        'label1
        '
        Me.label1.Location = New System.Drawing.Point(32, 168)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(152, 16)
        Me.label1.TabIndex = 0
        Me.label1.Text = "Data from client"
        '
        'label2
        '
        Me.label2.Location = New System.Drawing.Point(32, 120)
        Me.label2.Name = "label2"
        Me.label2.Size = New System.Drawing.Size(136, 16)
        Me.label2.TabIndex = 0
        Me.label2.Text = "Data to client"
        '
        'lServiceStatus
        '
        Me.lServiceStatus.Location = New System.Drawing.Point(136, 24)
        Me.lServiceStatus.Name = "lServiceStatus"
        Me.lServiceStatus.Size = New System.Drawing.Size(344, 16)
        Me.lServiceStatus.TabIndex = 0
        Me.lServiceStatus.Text = "Service not active"
        '
        'lStackID
        '
        Me.lStackID.Location = New System.Drawing.Point(32, 488)
        Me.lStackID.Name = "lStackID"
        Me.lStackID.Size = New System.Drawing.Size(232, 16)
        Me.lStackID.TabIndex = 0
        '
        'listSessions
        '
        Me.listSessions.Location = New System.Drawing.Point(320, 136)
        Me.listSessions.Name = "listSessions"
        Me.listSessions.Size = New System.Drawing.Size(152, 82)
        Me.listSessions.TabIndex = 0
        '
        'label3
        '
        Me.label3.Location = New System.Drawing.Point(320, 112)
        Me.label3.Name = "label3"
        Me.label3.Size = New System.Drawing.Size(88, 16)
        Me.label3.TabIndex = 0
        Me.label3.Text = "Sessions:"
        '
        'label4
        '
        Me.label4.Location = New System.Drawing.Point(32, 368)
        Me.label4.Name = "label4"
        Me.label4.Size = New System.Drawing.Size(136, 16)
        Me.label4.TabIndex = 0
        Me.label4.Text = "Data to client"
        '
        'label5
        '
        Me.label5.Location = New System.Drawing.Point(32, 416)
        Me.label5.Name = "label5"
        Me.label5.Size = New System.Drawing.Size(152, 16)
        Me.label5.TabIndex = 0
        Me.label5.Text = "Data from client"
        '
        'txtRead2
        '
        Me.txtRead2.Location = New System.Drawing.Point(32, 432)
        Me.txtRead2.Name = "txtRead2"
        Me.txtRead2.Size = New System.Drawing.Size(216, 20)
        Me.txtRead2.TabIndex = 0
        Me.txtRead2.Text = ""
        '
        'bWrite2
        '
        Me.bWrite2.Location = New System.Drawing.Point(256, 384)
        Me.bWrite2.Name = "bWrite2"
        Me.bWrite2.Size = New System.Drawing.Size(56, 24)
        Me.bWrite2.TabIndex = 0
        Me.bWrite2.Text = "Write"
        '
        'bAdvertise2
        '
        Me.bAdvertise2.Location = New System.Drawing.Point(24, 272)
        Me.bAdvertise2.Name = "bAdvertise2"
        Me.bAdvertise2.Size = New System.Drawing.Size(96, 24)
        Me.bAdvertise2.TabIndex = 0
        Me.bAdvertise2.Text = "Advertise"
        '
        'txtWriteData2
        '
        Me.txtWriteData2.Location = New System.Drawing.Point(32, 384)
        Me.txtWriteData2.Name = "txtWriteData2"
        Me.txtWriteData2.Size = New System.Drawing.Size(216, 20)
        Me.txtWriteData2.TabIndex = 0
        Me.txtWriteData2.Text = "some data2"
        '
        'bDeadvertise2
        '
        Me.bDeadvertise2.Location = New System.Drawing.Point(24, 304)
        Me.bDeadvertise2.Name = "bDeadvertise2"
        Me.bDeadvertise2.Size = New System.Drawing.Size(96, 24)
        Me.bDeadvertise2.TabIndex = 0
        Me.bDeadvertise2.Text = "Deadvertise"
        '
        'lServiceStatus2
        '
        Me.lServiceStatus2.Location = New System.Drawing.Point(136, 272)
        Me.lServiceStatus2.Name = "lServiceStatus2"
        Me.lServiceStatus2.Size = New System.Drawing.Size(352, 16)
        Me.lServiceStatus2.TabIndex = 0
        Me.lServiceStatus2.Text = "Service not active"
        '
        'listSessions2
        '
        Me.listSessions2.Location = New System.Drawing.Point(320, 384)
        Me.listSessions2.Name = "listSessions2"
        Me.listSessions2.Size = New System.Drawing.Size(152, 82)
        Me.listSessions2.TabIndex = 0
        '
        'label8
        '
        Me.label8.Location = New System.Drawing.Point(320, 360)
        Me.label8.Name = "label8"
        Me.label8.Size = New System.Drawing.Size(88, 16)
        Me.label8.TabIndex = 0
        Me.label8.Text = "Sessions:"
        '
        'txtSCN
        '
        Me.txtSCN.Location = New System.Drawing.Point(240, 56)
        Me.txtSCN.Name = "txtSCN"
        Me.txtSCN.Size = New System.Drawing.Size(40, 20)
        Me.txtSCN.TabIndex = 0
        Me.txtSCN.Text = "0"
        '
        'txtSCN2
        '
        Me.txtSCN2.Location = New System.Drawing.Point(240, 304)
        Me.txtSCN2.Name = "txtSCN2"
        Me.txtSCN2.Size = New System.Drawing.Size(40, 20)
        Me.txtSCN2.TabIndex = 0
        Me.txtSCN2.Text = "0"
        '
        'cbSerialPort
        '
        Me.cbSerialPort.Location = New System.Drawing.Point(144, 56)
        Me.cbSerialPort.Name = "cbSerialPort"
        Me.cbSerialPort.Size = New System.Drawing.Size(80, 16)
        Me.cbSerialPort.TabIndex = 0
        Me.cbSerialPort.Text = "Serial Port"
        '
        'cbSerialPort2
        '
        Me.cbSerialPort2.Location = New System.Drawing.Point(144, 304)
        Me.cbSerialPort2.Name = "cbSerialPort2"
        Me.cbSerialPort2.Size = New System.Drawing.Size(80, 16)
        Me.cbSerialPort2.TabIndex = 0
        Me.cbSerialPort2.Text = "Serial Port"
        '
        'bCloseSession2
        '
        Me.bCloseSession2.Location = New System.Drawing.Point(320, 472)
        Me.bCloseSession2.Name = "bCloseSession2"
        Me.bCloseSession2.Size = New System.Drawing.Size(64, 24)
        Me.bCloseSession2.TabIndex = 0
        Me.bCloseSession2.Text = "Close"
        '
        'bCloseSession1
        '
        Me.bCloseSession1.Location = New System.Drawing.Point(320, 224)
        Me.bCloseSession1.Name = "bCloseSession1"
        Me.bCloseSession1.Size = New System.Drawing.Size(64, 24)
        Me.bCloseSession1.TabIndex = 0
        Me.bCloseSession1.Text = "Close"
        '
        'Form1
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(504, 525)
        Me.Controls.Add(Me.bCloseSession1)
        Me.Controls.Add(Me.bCloseSession2)
        Me.Controls.Add(Me.cbSerialPort)
        Me.Controls.Add(Me.txtSCN2)
        Me.Controls.Add(Me.txtSCN)
        Me.Controls.Add(Me.txtRead)
        Me.Controls.Add(Me.txtWriteData)
        Me.Controls.Add(Me.txtRead2)
        Me.Controls.Add(Me.txtWriteData2)
        Me.Controls.Add(Me.label3)
        Me.Controls.Add(Me.listSessions)
        Me.Controls.Add(Me.lStackID)
        Me.Controls.Add(Me.lServiceStatus)
        Me.Controls.Add(Me.label2)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.bWrite)
        Me.Controls.Add(Me.bDeadvertise)
        Me.Controls.Add(Me.bAdvertise)
        Me.Controls.Add(Me.label4)
        Me.Controls.Add(Me.label5)
        Me.Controls.Add(Me.bWrite2)
        Me.Controls.Add(Me.bAdvertise2)
        Me.Controls.Add(Me.bDeadvertise2)
        Me.Controls.Add(Me.lServiceStatus2)
        Me.Controls.Add(Me.listSessions2)
        Me.Controls.Add(Me.label8)
        Me.Controls.Add(Me.cbSerialPort2)
        Me.Name = "Form1"
        Me.Text = "SimpleService VB"
        Me.ResumeLayout(False)

    End Sub

#End Region

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then

            If Not (components Is Nothing) Then

                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ' You can get a valid evaluation key for BlueTools at
        ' http:'franson.com/bluetools/
        ' That key will be valid for 14 days. Just cut and paste that key into the statement below.
        ' To get a key that do not expire you need to purchase a license
        Dim license As New License
        license.LicenseKey = "WoK6IL94B9ECLOPQSXVZPixRDRfuIYHYWsK6"


        Try
            m_manager = Manager.GetManager()

            Select Case m_manager.StackID
                Case StackID.STACK_MICROSOFT
                    lStackID.Text = "Microsoft BT stack"
                Case StackID.STACK_WIDCOMM
                    lStackID.Text = "WidComm BT stack"
                Case Else
                    lStackID.Text = "Unknown"
            End Select

            ' Call any events in GUI thread!
            m_manager.Parent = Me

            'Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
            m_network = m_manager.Networks(0)

            ' Buttons
            bAdvertise.Enabled = True
            bDeadvertise.Enabled = False

            bAdvertise2.Enabled = True
            bDeadvertise2.Enabled = False

        Catch ex As Exception
            MessageBox.Show(ex.Message)
        End Try
    End Sub


    Private Sub m_service_ClientConnected(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_service.ClientConnected, m_service2.ClientConnected


        Dim connectionEvent As ConnectionEventArgs = CType(eventArgs, ConnectionEventArgs)
        Dim connectedSession As Session = connectionEvent.Session

        If Not m_service Is Nothing Then
            If connectedSession.LocalService.Equals(m_service) Then
                listSessions.Items.Add(connectedSession)
                lServiceStatus.Text = "Client connected"

            Else
                If connectedSession.LocalService.Equals(m_service2) Then
                    listSessions2.Items.Add(connectedSession)
                    lServiceStatus2.Text = "Client connected"
                Else
                    lServiceStatus2.Text = "ERROR!"
                End If
            End If
        End If


        Dim connectedStream As System.IO.Stream = connectedSession.Stream
        Dim buffer(20) As Byte

        connectedStream.BeginRead(buffer, 0, buffer.Length, New AsyncCallback(AddressOf readCallback), connectedStream)

    End Sub

    Private Sub m_service_ClientDisconnected(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs)
        Dim connectionEvent As ConnectionEventArgs = CType(eventArgs, ConnectionEventArgs)
        Dim connectedSession As Session = connectionEvent.Session
        If connectedSession.LocalService.Equals(m_service) Then
            listSessions.Items.Remove(connectedSession)
            lServiceStatus.Text = "Client disconnected"
        Else
            If connectedSession.LocalService.Equals(m_service2) Then
                listSessions2.Items.Remove(connectedSession)
                lServiceStatus2.Text = "Client disconnected"
            Else
                lServiceStatus2.Text = "Client disconnected ERROR"
            End If
        End If
    End Sub

    Private Sub bAdvertise_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles bAdvertise.Click ', bAdvertise2.Click
        Try

            ' One more local service
            If (cbSerialPort.Checked = True) Then
                m_service = New LocalService(ServiceType.SerialPort, "SerialPort", "")
            Else
                m_service = New LocalService(ServiceType.RFCOMM, "RFCOMM", "")
            End If

            m_network.Server.Advertise(m_service, Integer.Parse(txtSCN.Text))

        Catch ex As Exception
            MessageBox.Show(ex.ToString())
        End Try
    End Sub

    Private Sub bDeadvertise_Click_1(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles bDeadvertise.Click ', bDeadvertise2.Click
        Try
            m_network.Server.Deadvertise(m_service)
        Catch ex As Exception
            MessageBox.Show(ex.ToString)
        End Try
    End Sub

    Private Sub m_service_Advertised(ByVal sender As Object, ByVal eventArgs As Franson.blueTools.BlueToolsEventArgs) Handles m_service.Advertised, m_service2.Advertised

        If (m_service Is CType(sender, LocalService)) Then
            lServiceStatus.Text = "Service advertised SCN: " + m_service.Address.ServiceChannelNumber.ToString()

            ' Buttons
            bAdvertise.Enabled = False
            bDeadvertise.Enabled = True
        ElseIf (m_service2 Is CType(sender, LocalService)) Then

            lServiceStatus2.Text = "Service advertised SCN: " + m_service2.Address.ServiceChannelNumber.ToString()

            ' Buttons
            bAdvertise2.Enabled = False
            bDeadvertise2.Enabled = True
        End If
    End Sub

    Private Sub m_service_Deadvertised(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs) Handles m_service.Deadvertised, m_service2.Deadvertised

        If (m_service.Equals(CType(sender, LocalService))) Then
            lServiceStatus.Text = "Service not active"

            ' Buttons
            bAdvertise.Enabled = True
            bDeadvertise.Enabled = False
        ElseIf (m_service2.Equals(CType(sender, LocalService))) Then
            lServiceStatus2.Text = "Service not active"

            ' Buttons
            bAdvertise2.Enabled = True
            bDeadvertise2.Enabled = False
        End If
    End Sub

    Private Sub bWrite_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles bWrite.Click
        writeToSession(CType(listSessions.SelectedItem, Session))
    End Sub

    Private Sub writeToSession(ByVal selectedSession As Session)
        If Not (selectedSession Is Nothing) Then

            Dim selectedStream As System.IO.Stream = selectedSession.Stream
            Dim charWrite As Char() = txtWriteData.Text.ToCharArray()
            Dim byteWrite(charWrite.Length) As Byte
            Dim inx As Integer = 0

            While inx < charWrite.Length
                byteWrite(inx) = Microsoft.VisualBasic.AscW(charWrite(inx))
                System.Math.Min(System.Threading.Interlocked.Increment(inx), inx - 1)
            End While

            selectedStream.BeginWrite(byteWrite, 0, byteWrite.Length, New AsyncCallback(AddressOf writeCallback), selectedStream)

        Else
            MessageBox.Show("Select a session first")
        End If
    End Sub

    Private Sub writeToSession2(ByVal selectedSession As Session)
        If Not (selectedSession Is Nothing) Then

            Dim selectedStream As System.IO.Stream = selectedSession.Stream
            Dim charWrite As Char() = txtWriteData2.Text.ToCharArray()
            Dim byteWrite(charWrite.Length) As Byte
            Dim inx As Integer = 0

            While inx < charWrite.Length
                byteWrite(inx) = Microsoft.VisualBasic.AscW(charWrite(inx))
                System.Math.Min(System.Threading.Interlocked.Increment(inx), inx - 1)
            End While

            selectedStream.BeginWrite(byteWrite, 0, byteWrite.Length, New AsyncCallback(AddressOf writeCallback), selectedStream)

        Else
            MessageBox.Show("Select a session first")
        End If
    End Sub

    Private Sub readCallback(ByVal result As IAsyncResult)
        Dim blueResults As BlueToolsAsyncResult = CType(result, BlueToolsAsyncResult)
        Dim currentStream As System.IO.Stream = CType(blueResults.AsyncState, System.IO.Stream)
        Dim buffer As Byte() = blueResults.buffer
        Dim len As Integer = currentStream.EndRead(result)
        Dim enc As System.Text.ASCIIEncoding = New System.Text.ASCIIEncoding
        Dim str As String = enc.GetString(buffer, 0, len)

        txtRead.Text = str
        currentStream.BeginRead(buffer, 0, buffer.Length, New AsyncCallback(AddressOf readCallback), currentStream)
    End Sub

    Private Sub writeCallback(ByVal result As IAsyncResult)
        Dim selectedStream As System.IO.Stream = CType(result.AsyncState, System.IO.Stream)

        ' EndWrite() must always be called if BegineWrite() was used!
        selectedStream.EndWrite(result)
    End Sub

    Private Sub bAdvertise2_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles bAdvertise2.Click

        ' One more local service
        If (cbSerialPort2.Checked = True) Then
            m_service2 = New LocalService(ServiceType.SerialPort, "SerialPort2", "")
        Else
            m_service2 = New LocalService(ServiceType.RFCOMM, "RFCOMM2", "")
        End If

        Try
            m_network.Server.Advertise(m_service2, Integer.Parse(txtSCN2.Text))

        Catch ex As Exception
            MessageBox.Show(ex.ToString())
        End Try

    End Sub

    Private Sub bDeadvertise2_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles bDeadvertise2.Click
        Try
            m_network.Server.Deadvertise(m_service2)
        Catch ex As Exception
            MessageBox.Show(ex.ToString())
        End Try
    End Sub

    Private Sub bWrite2_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles bWrite2.Click
        writeToSession2(CType(listSessions2.SelectedItem, Session))
    End Sub

    Private Sub bCloseSession1_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles bCloseSession1.Click
        closeSession(CType(listSessions.SelectedItem, Session))
        listSessions.Items.Remove(listSessions.SelectedItem)

    End Sub

    Private Sub closeSession(ByVal selectedSession As Session)
        If Not (selectedSession Is Nothing) Then
            selectedSession.Close()
        Else
            MessageBox.Show("Select a session first!")
        End If
    End Sub

    Private Sub listSessions_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles listSessions.SelectedIndexChanged

    End Sub

    Private Sub listSessions2_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles listSessions2.SelectedIndexChanged

    End Sub

    Private Sub Form1_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        Manager.GetManager().Dispose()
    End Sub

    Private Sub bCloseSession2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles bCloseSession2.Click
        closeSession(CType(listSessions2.SelectedItem, Session))
        listSessions2.Items.Remove(listSessions2.SelectedItem)
    End Sub
End Class
