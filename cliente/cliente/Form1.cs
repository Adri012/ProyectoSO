using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Collections;


namespace cliente
{
    public partial class Form1 : Form
    {
        Socket serv;
        Thread Atender;

        delegate void DelegadorParaEscribir(string mensaje);
        delegate void DelegadoGB(GroupBox mensaje);
        delegate void DelegadaDGV(DataGridView mensaje);

        int puerto = 50032;

        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            lbl_email.Hide();
            tB_email.Hide();
        }

        private void AtenderServidor()
        {
            while (true)
            {
                if (serv.Connected == false)
                    break;
                byte[] msg2 = new byte[200];
                serv.Receive(msg2);
                string mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(mensaje);
                string[] trozos = mensaje.Split('/');
                int codigo = Convert.ToInt32(trozos[0]);

                switch (codigo)
                {
                    case 0:     //PARTIDAS GANADAS
                        MessageBox.Show(trozos[1]);
                        serv.Shutdown(SocketShutdown.Both);
                        serv.Close();
                        Close();
                        break;
                    case 1:     //PARTIDAS GANADAS
                        MessageBox.Show(tB_peticion.Text + " ha ganado " + trozos[1] + " partidas");
                        break;
                    case 2:     //PARTIDAS JUGADAS
                        MessageBox.Show(tB_peticion.Text + " ha jugado " + trozos[1] + " partidas");
                        break;
                    case 4:     //REGISTRO
                        if (trozos[1] == "CORRECTO")
                            MessageBox.Show("Ya estas registrado");
                        else
                            MessageBox.Show("Registro incorrecto");
                        break;
                    case 5:     //INICIO SESION
                        if (trozos[1] == "INCORRECTO")
                            MessageBox.Show("Nombre de usuario o contraseña incorrecto");
                        else
                            MessageBox.Show("Bienvenido " + tB_nombre.Text);
                        break;
                    case 6:     //LISTA CONECTADOS
                        dgv_conectados.Rows.Clear();
                        int num = Convert.ToInt32(trozos[1]);
                        if (num > 0)
                        {
                            for (int i = 0; i < num; i++)
                                {
                                    dgv_conectados.RowCount = num;
                                    dgv_conectados.ColumnCount = 1;
                                    dgv_conectados.Rows[i].Cells[0].Value = trozos[i+2];
                                }
                        }
                        
                        break;
                    case 7:     //SERVICIOS
                        lbl_contar.Text = trozos[1];
                        break;
                }
            }
        }

        private void btn_registro_Click(object sender, EventArgs e)
        {
            lbl_email.Show();
            tB_email.Show();

            if (tB_email.Text == "")
            {
                MessageBox.Show("Introduzca su email para completar el registro.");
            }
            else
            {
                string reg = "4/" + tB_nombre.Text + "/" + tB_contrasena.Text + "/" + tB_email.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }
        }

        private void btn_Iniciar_Click(object sender, EventArgs e)
        {
            string reg = "5/" + tB_nombre.Text + "/" + tB_contrasena.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);

            byte[] res = new byte[150];
            serv.Receive(res);
            reg = System.Text.Encoding.ASCII.GetString(res).Split('\0')[0];
            if (reg == "SI")
            {
                MessageBox.Show("Inicio completado");
            }
            else
            {
                MessageBox.Show("No se ha podido iniciar");
            }
        }

        private void btn_Olvidado_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Aun no se ha implementado xd");
        }

        private void btn_salir_Click(object sender, EventArgs e)
        {
            string cod = "0/" + tB_nombre.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(cod);
            serv.Send(msg);
        }

        private void btn_enviar_Click(object sender, EventArgs e)
        {
            //Petición partidas ganadas por el usuario seleccionado (1)
            if (cbx_ganadas.Checked)
            {
                string reg = "1/" + tB_peticion.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }

            //Petición de partidas jugadas por el usuario (2)
            if (cbx_jugadas.Checked)
            {
                string reg = "2/" + tB_peticion.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }

            if (cbx_ganador.Checked)
            {
                string reg = "3/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(reg);
                serv.Send(msg);
            }

            if (cbx_ganadas.Checked==false && cbx_jugadas.Checked==false && cbx_ganador.Checked==false)
            {
                MessageBox.Show("Error en la petición");
            }
        }

        private void btn_conectar_Click(object sender, EventArgs e)
        {
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, puerto);
            serv = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                serv.Connect(ipep);
                MessageBox.Show("Bienvenido");
            }
            catch (SocketException)
            {
                MessageBox.Show("Error de socket");
                return;
            }

            ThreadStart ts = delegate { AtenderServidor(); };
            Atender = new Thread(ts);
            Atender.Start();


            //string reg6 = "6/";
            //byte[] msg6 = System.Text.Encoding.ASCII.GetBytes(reg6);
            //serv.Send(msg6);

            if (tB_nombre.Text=="")
            {
                MessageBox.Show("Introduzca nombre de usuario");
            }
            else
            {
                string reg9 = "9/" + tB_nombre.Text;
                MessageBox.Show(reg9);
                byte[] msg9 = System.Text.Encoding.ASCII.GetBytes(reg9);
                serv.Send(msg9);
            }
            
        }
    }
}