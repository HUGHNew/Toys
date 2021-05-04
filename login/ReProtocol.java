package login;


import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;

abstract public class ReProtocol {
    /** length
     * Header and each parts' length bytesHeader and each parts' length
     * bytes
    */
    static public final Integer
        HeaderLen=8,
        MsgLen=4,
        CmdLen=4,
        StatusCodeLen=1,
        DescriptionLen=64,
        UserNameLen=20,
        UserPswLen=30,
        RequestLen=58,
        ResponseLen=73
    ;
    static public final byte[]
        BytesRReq={0,0,0,1},
        BytesRRes={0,0,0,2},
        BytesLReq={0,0,0,3},
        BytesLRes={0,0,0,4},
        BytesReqHd={0,0,0,58}, // Request Header Length
        BytesResHd={0,0,0,73} // Response Header Length
    ;
    static public final String
        LRequest=RequestString(true,true),
        RRequest=RequestString(false,true),
        LResponse=RequestString(true,false),
        RResponse=RequestString(false,false)
    ;
    /**
    * @return Octet BytesString without inner filling ZERO
    * */
    static public String RequestString(boolean login,boolean request){
        char zero=0,len=0,id=0;
        int rd= request?58:73;
        int idd=request?1:2;
        if(login)idd+=2;
        // assign part
        len= (char) rd;
        id= (char) idd;
        return String.valueOf(zero)+len+zero+id;
    }
    public enum Command {
        registrationRequest,
        registrationResponse,
        loginRequest,
        loginResponse
    }
    public static class ReMsgBase{
        public int len,id;
        public ReMsgBase(int l,int cid){
            this.len=l;
            this.id=cid;
        }
        public ReMsgBase(int l,Command c){
            this.len=l;
            this.id=CommandToInt(c);
        }
    }
    public static int CommandToInt(Command c){
        return switch (c){
            case registrationRequest ->  1;
            case registrationResponse -> 2;
            case loginRequest -> 3;
            case loginResponse -> 4;
        };
    }
    public static Command IntToCommand(int value)throws RePException{
        return switch (value){
            case 1->Command.registrationRequest;
            case 2->Command.registrationResponse;
            case 3->Command.loginRequest;
            case 4->Command.loginResponse;
            default -> throw new RePException("Command not found!");
        };
    }
    protected Command type;
    protected Integer totalLen;
    public ReProtocol(Command cmd){
        this.type=cmd;
        switch (cmd){
            case loginRequest,registrationRequest ->
                    {totalLen=RequestLen;}
            case loginResponse,registrationResponse ->
                    {totalLen=ResponseLen;}
        }
    }
    static public int getValueFromMsg(byte[]data,int beg){
        int value=0;
        for (int i = 0; i < CmdLen; i++) {
            value+=(((int)data[i+beg])<<((7-i)*8));
        }
        return value;
    }
    abstract public byte[] getBytes() throws RePException;
    public int prefill(byte[] msg,Command cmd){
        int offset=0;
        System.arraycopy(ReProtocol.BytesLReq, 0, msg, offset, 3);
        offset+=4;
        System.arraycopy(ReProtocol.BytesReqHd, 0, msg, offset, 4);
        switch (cmd){
            case loginResponse -> {
                msg[MsgLen-1]=ResponseLen.byteValue();
                msg[HeaderLen-1]=4;
            }
            case loginRequest -> {
                msg[MsgLen-1]=RequestLen.byteValue();
                msg[HeaderLen-1]=3;
            }
            case registrationResponse -> {
                msg[MsgLen-1]=ResponseLen.byteValue();
                msg[HeaderLen-1]=2;
            }
            case registrationRequest -> {
                msg[MsgLen-1]=RequestLen.byteValue();
                msg[HeaderLen-1]=1;
            }
        }
        return HeaderLen;
    }
    protected byte[] fillBase(String field,int len){
        byte[] msg=new byte[len];
        byte[] Bin=field.getBytes(StandardCharsets.UTF_8);
        for (int i = 0; i < len; ++i) {
            if(i<Bin.length)
                msg[i]=Bin[i];
            else msg[i]=0;
        }return msg;
    }
    protected static byte[] decodeBase(byte[]filled){
        int newlen=0;
        byte[] fit=null;
        if(filled[filled.length-1]==0) {
            for (; filled[newlen] != 0; ++newlen) ;
            fit=new byte[newlen];
            System.arraycopy(filled, 0, fit, 0, newlen);
        }
        else fit=filled;
        return fit;
    }
    protected static String decode(byte[]raw, Charset set){
        return new String(raw,set);
    }
    protected static String decode(byte[]raw){
        return new String(raw,StandardCharsets.UTF_8);
    }
    protected static String decodeFill(byte[]fill){
        return decode(decodeBase(fill));
    }
}
