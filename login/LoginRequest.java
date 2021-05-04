package login;


public class LoginRequest extends Request{

    public LoginRequest(String userName,String userPassword){
        super(Command.loginRequest,userName,userPassword);
    }
    public static byte[] WholeMsg(Command cmd,String userName,String userPassword)
            throws RePException{
        return new LoginRequest(userName, userPassword).getBytes();
    }
}
