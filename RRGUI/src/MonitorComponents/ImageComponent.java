/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package MonitorComponents;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.DataBufferUShort;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import javax.swing.JPanel;

public class ImageComponent extends JPanel{

    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private BufferedImage image= null;
    private int width= 0;
    private int height= 0;
    
    public void setImageSize(int _w, int _h) {
        height= _h;
        width= _w;
    }

    public void set8bitGrayImage(byte [] bytes) {
        if (bytes == null) return;
        if (image == null || image.getType() != BufferedImage.TYPE_BYTE_GRAY) {
            image= new BufferedImage(width, height, BufferedImage.TYPE_BYTE_GRAY);
        }
        
        System.arraycopy(bytes, 0, ((DataBufferByte)image.getRaster().getDataBuffer()).getData(), 0, bytes.length) ;
    }

    
    public void set16bitGrayImage(byte [] bytes) {
        if (bytes == null) return;
        if (image == null || image.getType() != BufferedImage.TYPE_USHORT_GRAY) {
            image= new BufferedImage(width, height, BufferedImage.TYPE_USHORT_GRAY);
        }
        
        short img[]= ((DataBufferUShort)image.getRaster().getDataBuffer()).getData();
        ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(img);

        float min= 65536, max= -65536;
        final int n= img.length;
        for (int i= 0; i<n; i++) {
            short data= img[i];
            if (data < min) min= data;
            if (data > max) max= data;
        }
        for (int i= 0; i<n; i++) {
            img[i]= (short) (65535*(img[i]-min)/(max-min));
        }

//short [] sh2= ((DataBufferUShort)image.getRaster().getDataBuffer()).getData();
//int min= 65536;
//int max= -65536;
//int sh[]= new int[bytes.length/2];
//for (int i= 0; i<sh.length; i++) {
//    int a= Byte.toUnsignedInt(bytes[2*i+1]);
//    int b= Byte.toUnsignedInt(bytes[2*i]);
//    
//    int s=  a<<8| b;
//    if (s>max)
//        max=s;
//    if (s<min)
//        min=s;
//    sh[i]= s;
//}
//for (int i= 0; i<sh.length; i++) {
//    sh2[i]= (short) (65535*(sh[i]-min)/(max-min));
//}

        //System.arraycopy(bytes, 0, ((DataBufferUShort)image.getRaster().getDataBuffer()).getData(), 0, bytes.length/2) ;

    }

    
    @Override
    protected void paintComponent(Graphics g) {

        super.paintComponent(g);
        
        if (image == null) return;
        g.drawImage(image, 0, 0, this); 
    }

}