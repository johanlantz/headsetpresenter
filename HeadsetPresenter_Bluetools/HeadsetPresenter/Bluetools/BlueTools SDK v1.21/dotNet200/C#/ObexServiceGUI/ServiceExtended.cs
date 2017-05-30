using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Franson.Protocols.Obex.OBEXService;

namespace ObexServiceSampleCF
{
	/// <summary>
	/// Summary description for ServerExtended.
	/// </summary>
	public class ServiceExtended : Franson.Protocols.Obex.OBEXService.ObexService
	{
		//Add a couple of event handlers to communicate to the GUI with
		public delegate void ObexServiceEventHandler(ObexSession obexSessions);
		public delegate void ObexServiceFileTransferEventHandler(ObexSession obexSessions, ObexFile obexFile);
		public delegate void ObexServiceFolderOperationEventHandler(ObexSession obexSessions, ObexFolder obexFolder);
		public delegate void ObexServiceFolderItemEventHandler(ObexSession obexSessions, ObexFolderItem folderItem);
		public delegate void ObexServiceExceptionEventHandler(ObexSession obexSession, Exception exception);


		public event ObexServiceEventHandler				OnConnect;
		public event ObexServiceEventHandler				OnDisconnect;
		public event ObexServiceFolderItemEventHandler		OnDelete;
		public event ObexServiceEventHandler				OnGetPath;
		public event ObexServiceFileTransferEventHandler	OnGet;
		public event ObexServiceFileTransferEventHandler	OnPut;
		public event ObexServiceFolderOperationEventHandler	OnCreateFolder;
		public event ObexServiceFolderOperationEventHandler	OnSetPath;
		public event ObexServiceEventHandler				OnAbort;
		public event ObexServiceExceptionEventHandler		OnObexException;

		public ServiceExtended(ObexServiceType obexServiceType, string name, string description, ObexFolder rootFolder) : base (obexServiceType, name, description, rootFolder)
		{
			
		}

		public override Response OnConnectRequest(ObexSession obexSession)
		{	
			OnConnect(obexSession);
			return base.OnConnectRequest (obexSession);
		}

		public override Response OnDisconnectRequest(ObexSession obexSession)
		{
			OnDisconnect(obexSession);
			return base.OnDisconnectRequest (obexSession);
		}

		public override Response OnDeleteRequest(ObexSession obexSession, ObexFolderItem folderItem)
		{
			OnDelete(obexSession, folderItem);
			return base.OnDeleteRequest (obexSession, folderItem);
		}

		public override Response OnFolderCreateRequest(ObexSession obexSession, ObexFolder folder)
		{
			OnCreateFolder(obexSession, folder);
			return base.OnFolderCreateRequest (obexSession, folder);
		}

		public override Response OnGetPathRequest(ObexSession obexSession, ObexFolderContent folderContent)
		{
			OnGetPath(obexSession);
			return base.OnGetPathRequest (obexSession, folderContent);
		}

		public override Response OnGetRequest(ObexSession obexSession, ObexFile file, ObexStream outStream)
		{
			OnGet(obexSession, file);		
			return base.OnGetRequest (obexSession, file, outStream);
		}

		public override Response OnPutRequest(ObexSession obexSession, ObexFile file, ObexStream inStream)
		{
			OnPut(obexSession, file);
			return base.OnPutRequest (obexSession, file, inStream);
		}

		public override Response OnSetPathRequest(ObexSession obexSession, ObexFolder folderToChangeTo)
		{
			OnSetPath(obexSession, folderToChangeTo);
			return base.OnSetPathRequest (obexSession, folderToChangeTo);
		}

		public override Response OnAbortRequest(ObexSession obexSession)
		{
			OnAbort(obexSession);
			return base.OnAbortRequest (obexSession);
		}
		
		public override void OnOBEXServerException(ObexSession obexSession, object sender, Exception exception)
		{
			base.OnOBEXServerException (obexSession, sender, exception);
		
			OnObexException(obexSession, exception);
		}
	}
}
