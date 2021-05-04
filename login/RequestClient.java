package login;

import java.io.*;
import java.net.Socket;

public class RequestClient implements AutoCloseable{
    private Socket socket;
    private Request.ReqMsg request=null;
    private Response.ResMsg response=null;
    public static void main(String[]args){
//        UserInterface();
        test(true, "James", "123");
        test(false, "Jame", "123");
        test(false, "Zhangsan", "123");
        test(true, "Zhangsan", "123");
        test(false, "Tom", "123");
        test(false, "ALias", "123");
        test(true, "ALias", "123");
    }
    public static boolean test(boolean login,String name,String psw){
        RequestClient Client=new RequestClient();
        Client.loadReq(login, name,psw);
        try {
            if(login)
                System.out.println("登录结果:"+Client.request());
            else System.out.println("注册结果:"+Client.request());
        } catch (RePException e) {
            return false;
        }return true;
    }
    private static boolean UIcheck(String str){
        return str.length() == 1 && str.charAt(0) < 3;
    }
    public static RequestClient UserInterface(){
        String name=null,pass=null;
        boolean login=false;
        System.out.println("0 -- 登录\n1 -- 注册\n2 -- 退出\n输入后按回车继续");
        BufferedReader reader=new BufferedReader(new InputStreamReader(System.in));
        try {
            String tmp=reader.readLine();
            while((tmp.length()!=1||tmp.equals(""))&&!UIcheck(tmp)){
                System.out.println("选项不存在");
                tmp=reader.readLine();
            }
            if(tmp.charAt(0)==2)System.exit(0);
            login=tmp.charAt(0)==0;
            name=reader.readLine();
            while(name==null||name.equals("")){
                System.out.println("姓名不能为空 请重新输入");
                name=reader.readLine();

            }
            pass=reader.readLine();
            while(pass==null||pass.equals("")){
                System.out.println("密码不能为空 请重新输入");
                pass=reader.readLine();
            }
            return new RequestClient(login,name,pass);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
    public RequestClient(boolean login,String name,String passwd){
        this("127.0.0.1",12345,login,name,passwd);
    }
    public RequestClient(String ipaddr, int port,boolean login,String name,String passwd){
        try {
            this.socket=new Socket(ipaddr,port);
            loadReq(login,name,passwd);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public RequestClient(){
        this("127.0.0.1",ResponseServer.DefaultPort);
    }
    public RequestClient(String ipaddr, int port){
        try {
            this.socket=new Socket(ipaddr,port);
        } catch (IOException e) {
            // todo
            e.printStackTrace();
        }
    }
    public boolean loadReq(boolean login,String name,String passwd){
        ReProtocol.Command cmd=login?
                ReProtocol.Command.loginRequest: ReProtocol.Command.registrationRequest;
        if(passwd.isEmpty())return false;
        request= new Request.ReqMsg(cmd,name,passwd);
        return true;
    }
    public String request()throws RePException{
        if(socket==null)throw new RePException("socket is not connected");
        try {
            return send().recv();
//            String rel=send().recv();
//            System.out.println(response.statusCode);
//            return rel;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }
    protected RequestClient send() throws IOException, RePException {
        DataOutputStream BOS=new DataOutputStream(socket.getOutputStream());
        if(request==null)throw new IOException("uninitialized request");
        Request req=null;
        if(request.id==3){ // login
            req=new LoginRequest(request.name, request.psw);
        }else{
            req=new RegistrationRequest(request.name, request.psw);
        }
        BOS.write(req.getBytes());
        BOS.flush();
        return this;
    }
    protected String recv() throws IOException, RePException {
        DataInputStream BIS=new DataInputStream(socket.getInputStream());
        byte[] resp=new byte[ReProtocol.ResponseLen];
        int len=BIS.read(resp);
        response=Response.getMsg(resp);
        BIS.close();
        return response.desc;
    }

    @Override
    public void close() throws Exception {
        socket.close();
    }
}