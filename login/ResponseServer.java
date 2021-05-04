package login;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.TreeMap;
import java.security.*;

public class ResponseServer implements Serializable,AutoCloseable{
    Map<String,String>database,added;
    private ServerSocket listener;
    static public final int DefaultPort=12345;
    protected static final String DATAPATH="passwd";
    protected final File passwd=new File(DATAPATH);
    public static void main(String[] args){
        ResponseServer Server=new ResponseServer();
//        Timer clock=new Timer("AutoSave");
//        clock.schedule(new AutoSaveTimerTask(Server) {
//            @Override
//            public void run() {
//                this.obj.flush(0); // problem here
//                //todo
//            }
//        }, 100);
        int tid=0;
        while(true){
            try {
                Socket soc=Server.accept();
                new Thread(new ResponseWorker(soc,Server,true),
                        "Thread:"+tid++).start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    public Socket accept() throws IOException {
        return listener.accept();
    }
    public ResponseServer(){
        database=new TreeMap<>();
        added=new TreeMap<>();
        try {
            listener=new ServerSocket(DefaultPort);
        } catch (IOException e) {
            e.printStackTrace();
        }
        if(!passwd.exists()){
            try {
                passwd.createNewFile();
            } catch (IOException e) {
                // todo
            }
        }else{
            reload(passwd,0);
        }
    }
    public void reload(File src,int method){
        switch (method){
            case 1->{
                loadFromSerialization(src);
            }
            default -> {
                loadFromText(src,false);
            }
        }
    }
    protected void loadFromText(File src,boolean show){
        try {
            BufferedReader reader=new BufferedReader(new FileReader(src));
            String line=null;
            while((line=reader.readLine())!=null){
                String[]rels=line.split("\\|",2);
                database.put(rels[0],rels[1]);
                if(show) System.out.println(rels[0]+"|"+rels[1]);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    protected void loadFromSerialization(File src){
        try {
            FileInputStream fileIn=new FileInputStream(src);
            ObjectInputStream objIn=new ObjectInputStream(fileIn);
            database=(TreeMap<String,String>)objIn.readObject();
        } catch (IOException | ClassNotFoundException e) {
            // todo
            // impossible here
        }
    }
    public void flush(int method){
        switch (method){
            case 1->{
                saveBySerialization();
            }
            default -> {
                saveByText();
            }
        }
    }
    protected void saveByText(){
        try {
            BufferedWriter writer=new BufferedWriter(new FileWriter(DATAPATH,true));
            synchronized (this){
                for (Map.Entry<String, String> entry : added.entrySet()) {
                    writer.write(entry.getKey() + " " + entry.getValue()+"\n");
                    System.out.println(entry.getKey() + " " + entry.getValue());
                }
                added.clear();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    protected void saveBySerialization(){
        try {
            FileOutputStream fileOut=new FileOutputStream(passwd);
            ObjectOutputStream objOut=new ObjectOutputStream(fileOut);
            objOut.writeObject(database);
            objOut.flush();fileOut.flush();
            objOut.close();fileOut.close();
        } catch (IOException e) {
            // todo
            // impossible here
        }
    }
    static public String md5(String raw){
        try {
            MessageDigest md=MessageDigest.getInstance("MD5");
            md.update(raw.getBytes(StandardCharsets.UTF_8));
            return new String(md.digest(), StandardCharsets.UTF_8);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
            // impossible here
        }
        return raw;
    }
    static public int queryDescription(Map<String,String>database,String name,String psw,boolean login){
        String psd=database.get(name);
        if(login){
            if(psd==null)return Response.fall_user_name_no_exist;
            if(psd.equals(md5(psw)))return Response.succ_login;
            else return Response.fall_no_match;
        }else{
            if(psd==null) {
//                database.put(name,psw);
                return Response.succ_registration;
            }else { // error
                return Response.fall_user_name_repeated;
            }
        }
    }

    @Override
    public void close() throws Exception {
        flush(0);
        listener.close();
    }
}



class ResponseWorker implements Runnable,AutoCloseable{
    Socket socket;
    final ResponseServer rserver;
    Response.ResMsg response=null;
    Request.ReqMsg request=null;
    boolean autoSave;
    public ResponseWorker(Socket client,ResponseServer rs,boolean AutoSave){
        this.socket=client;
        this.rserver=rs;
        this.autoSave=AutoSave;
    }
    @Override
    public void run() {
        try {
            this.getRequest().getResponse().close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public ResponseWorker getRequest() throws IOException {
        byte[] req=new byte[ReProtocol.RequestLen];
        DataInputStream BIS=new DataInputStream(socket.getInputStream());
        int len=BIS.read(req);
        try {
            request=Request.getMsg(req);
        } catch (RePException e) {
            e.printStackTrace();
        }
//        finally {
//            BIS.close();
//        }
        return this;
    }
    public ResponseWorker getResponse() throws IOException {
        int index=ResponseServer.queryDescription(rserver.database, request.name, request.psw,
                request.id==3);
        ReProtocol.Command cmd=request.id==3?
                ReProtocol.Command.loginResponse:ReProtocol.Command.registrationResponse;
        response=new Response.ResMsg(cmd,index>2,index);
        DataOutputStream BOS=new DataOutputStream(socket.getOutputStream());
        try {
            BOS.write(new Response(response).getBytes());
            BOS.flush();
            if(index==1){ // registry
                push();
            }
            // BOS.close();
        } catch (RePException  e) {
            e.printStackTrace();
        }
        return this;
    }
    @Override
    public void close(){
        try {
            socket.close();
            if(!autoSave)return;
            try {
                BufferedWriter writer=new BufferedWriter(new FileWriter(ResponseServer.DATAPATH,true));
                if(rserver.added.size()!=0){
                    synchronized (this.rserver){
                        for (Map.Entry<String, String> entry : rserver.added.entrySet()) {
                            writer.write(entry.getKey() + "|" + entry.getValue()+"\n");
                        }writer.flush();
                        rserver.added.clear();
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void push(){
        synchronized (this.rserver){
            this.rserver.database.put(request.name,ResponseServer.md5(request.psw));
            this.rserver.added.put(request.name,ResponseServer.md5(request.psw));
        }
    }
}

abstract class AutoSaveTimerTask extends TimerTask{
    public ResponseServer obj;
    public AutoSaveTimerTask(ResponseServer svr){
        this.obj=svr;
    }
}