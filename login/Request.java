package login;


import java.nio.charset.StandardCharsets;

abstract public class Request extends ReProtocol{
    public static class ReqMsg extends ReMsgBase{
        public String name,psw;
        public ReqMsg(Command cid,String UserName,String PassWord) {
            super(RequestLen,cid);
            this.name=UserName;
            this.psw=PassWord;
        }
    }
    protected String name,psw;
    public Request(ReqMsg rmsg)throws RePException {
        this(IntToCommand(rmsg.id),rmsg.name,rmsg.psw);
    }
    public Request(Command cmd,String username,String userpasswd) {
        super(cmd);
        this.name=username;
        this.psw=userpasswd;
    }
    public ReqMsg getMsg(){
        return new ReqMsg(this.type,name,psw);
    }
    public static ReqMsg getMsg(byte[]data) throws RePException {
        if(data.length!=ReProtocol.RequestLen){
            throw new RePException("Wrong-format Request");
        }else{
            byte[] name=new byte[UserNameLen];
            byte[] pass=new byte[UserPswLen];
            System.arraycopy(data, HeaderLen, name, 0, HeaderLen);
            System.arraycopy(data, HeaderLen+UserNameLen, pass, 0, UserPswLen);
            return new ReqMsg(IntToCommand(getValueFromMsg(data, MsgLen)),decodeFill(name),decodeFill(pass));
        }
    }
    /**
     * @return 0 for OK
     * 1 : name too long
     * 2 : psw too long
     */
    public int dataValidate(){
        if(name.length()>UserNameLen/2){
            return 1;
        }
        if (psw.length()>UserPswLen/2){
            return 2;
        }
        return 0;
    }

    /**
     * @brief no length check here
     * @return bytes of UTF8
     * */
    public byte[] nameFill(){
        return fillBase(name, UserNameLen);
    }
    /**
     * @brief no length check here
     * @return bytes of UTF8
     * */
    public byte[] passWdFill(){
        return fillBase(psw, UserPswLen);
    }
    @Override
    public byte[] getBytes() throws RePException{
        byte[] msg=new byte[this.totalLen];
        switch (dataValidate()){
            case 1->{throw new RePException("User Name is too long");}
            case 2->{throw new RePException("User password is too long");}
            default -> {
                int offset=prefill(msg, type);
                System.arraycopy(nameFill(), 0, msg, offset, UserNameLen);
                offset+=20;
                System.arraycopy(passWdFill(), 0, msg, offset, UserPswLen);
            }
        }
        return msg;
    }
}
