using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;
using System.Net;
using Franson.Protocols.Obex.OBEXService;

namespace ObexServerSample
{
	/// <summary>
	/// A extended version of the ObexService to demonstrate tha ability  to override core functions and extetnds functionality.
	/// This version only adds events to pass information to the gui. 
	/// The stubs can easily be filled in with custom code. See the example in the code below. 
	/// 
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

		//Use the base constructor
		public ServiceExtended(ObexServiceType obexServiceType, string name, string description, ObexFolder rootFolder) : base (obexServiceType, name, description, rootFolder)
		{
			
		}

		#region Exmple on how to refuse connections from a device with a certain address 
		/*
		public override Response OnConnectRequest(ObexSession obexSession)
		{
			if(obexSession.RemoteDevice.Address.Equals(new Address("(00:0e:07:28:55:fd)")))
			{
				return Response.DENIED;
			}
    
			return Response.SUCCESS;
		}
		*/
		#endregion
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
		
		#region An example that demostrate how to deny any attempt to delete the file "README.TXT" 
		/*
		public override Response OnDeleteRequest(ObexSession obexSession, ObexFolderItem folderItem)
		{
			if(folderItem.Name == "README.TXT")
			{
				return Response.DENIED;
			}
			else
			{
				return base.OnDeleteRequest (obexSession, folderItem);
			}
		}
		*/
		#endregion
		public override Response OnDeleteRequest(ObexSession obexSession, ObexFolderItem folderItem)
		{
			OnDelete(obexSession, folderItem);
			return base.OnDeleteRequest (obexSession, folderItem);
		}
		
		#region An example that demonstrate how to deny clients to create directories.
		/*
		 private override  Response OnFolderCreateRequest(ObexSession obexSession, ObexFolder folder)
		{
			return Response.DENIED;
		}
		*/
		#endregion
		public override Response OnFolderCreateRequest(ObexSession obexSession, ObexFolder folder)
		{
			OnCreateFolder(obexSession, folder);
			return base.OnFolderCreateRequest (obexSession, folder);
		}

		#region An example that adds all files and directorys in the obexSession.CurrentFolder to the folderContent object 
		/*
		public override Response OnGetPathRequest(ObexSession obexSession, ObexFolderContent folderContent)
		{
			DirectoryInfo di = new DirectoryInfo(obexSession.CurrentFolder.Path);

			if(di.Exists)
			{
				//Go throug evry file and add them to folderContetn
				foreach(FileInfo f in di.GetFiles())
				{
					if(f.Exists)
					{
						//Create an ObexFile
						ObexFile file = new ObexFile(f.Name, f.FullName);
    
						//Set the attributes of the ObexFile
						file.Size = f.Length;
						file.Created = f.CreationTime;
						file.Accessed = f.LastAccessTime;
						file.Modified = f.LastWriteTime;
    
						//Add the ObexFile to folderContent
						folderContent.Add(file);
					}
				}
				//Go throug evry directory and add them to folderConent
				foreach(DirectoryInfo d in di.GetDirectories())
				{
					if(d.Exists)
					{
						//Create an ObexFolder
						ObexFolder folder = new ObexFolder(d.Name, d.FullName);

						//Set the attributes of the ObexFile
						folder.Created = d.CreationTime;
						folder.Accessed = d.LastAccessTime;
						folder.Modified = d.LastWriteTime;

						//Add the ObexFile to folderContent
						folderContent.Add(folder);
					}

				}
			}
			else
			{
				return Response.NOT_FOUND;
			}

			return response.ACCEPTED;    
		}
		*/
		#endregion
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
		
		#region An example that demonstrate how to restrict accsess to folders named "Upload".
		/*
		protected override Response OnSetPathRequest(ObexSession obexSession, ObexFolder folderToChangeTo)
		{
			DirectoryInfo di = new DirectoryInfo(folderToChangeTo.Path);
			
			//Check if folderToChangeTo exists and the name is not "Upload"
			if(di.Exists && di.Name != "Upload")
			{
				obexSession.CurrentFolder = folderToChangeTo;
				return Response.SUCCESS;
			}
			else
			{
				return Response.DENIED;
			}
		}
		*/
		#endregion
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
