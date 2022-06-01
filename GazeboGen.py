def CreateEmptyWorld():
    str_head = """
    <sdf version='1.7'>
      <world name='default'>
        <light name='sun' type='directional'>
          <cast_shadows>1</cast_shadows>
          <pose>0 0 10 0 -0 0</pose>
          <diffuse>0.8 0.8 0.8 1</diffuse>
          <specular>0.2 0.2 0.2 1</specular>
          <attenuation>
            <range>1000</range>
            <constant>0.9</constant>
            <linear>0.01</linear>
            <quadratic>0.001</quadratic>
          </attenuation>
          <direction>-0.5 0.1 -0.9</direction>
          <spot>
            <inner_angle>0</inner_angle>
            <outer_angle>0</outer_angle>
            <falloff>0</falloff>
          </spot>
        </light>
        <model name='ground_plane'>
          <static>1</static>
          <link name='link'>
            <collision name='collision'>
              <geometry>
                <plane>
                  <normal>0 0 1</normal>
                  <size>100 100</size>
                </plane>
              </geometry>
              <surface>
                <friction>
                  <ode>
                    <mu>100</mu>
                    <mu2>50</mu2>
                  </ode>
                  <torsional>
                    <ode/>
                  </torsional>
                </friction>
                <contact>
                  <ode/>
                </contact>
                <bounce/>
              </surface>
              <max_contacts>10</max_contacts>
            </collision>
            <visual name='visual'>
              <cast_shadows>0</cast_shadows>
              <geometry>
                <plane>
                  <normal>0 0 1</normal>
                  <size>100 100</size>
                </plane>
              </geometry>
              <material>
                <script>
                  <uri>file://media/materials/scripts/gazebo.material</uri>
                  <name>Gazebo/Grey</name>
                </script>
              </material>
            </visual>
            <self_collide>0</self_collide>
            <enable_wind>0</enable_wind>
            <kinematic>0</kinematic>
          </link>
        </model>
        <gravity>0 0 -9.8</gravity>
        <magnetic_field>6e-06 2.3e-05 -4.2e-05</magnetic_field>
        <atmosphere type='adiabatic'/>
        <physics type='ode'>
          <max_step_size>0.001</max_step_size>
          <real_time_factor>1</real_time_factor>
          <real_time_update_rate>1000</real_time_update_rate>
        </physics>
        <scene>
          <ambient>0.4 0.4 0.4 1</ambient>
          <background>0.7 0.7 0.7 1</background>
          <shadows>1</shadows>
        </scene>
        <wind/>
        <spherical_coordinates>
          <surface_model>EARTH_WGS84</surface_model>
          <latitude_deg>0</latitude_deg>
          <longitude_deg>0</longitude_deg>
          <elevation>0</elevation>
          <heading_deg>0</heading_deg>
        </spherical_coordinates>
        <state world_name='default'>
          <sim_time>51 539000000</sim_time>
          <real_time>51 751115600</real_time>
          <wall_time>1646516655 282359600</wall_time>
          <iterations>51539</iterations>
          <model name='ground_plane'>
            <pose>0 0 0 0 -0 0</pose>
            <scale>1 1 1</scale>
            <link name='link'>
              <pose>0 0 0 0 -0 0</pose>
              <velocity>0 0 0 0 -0 0</velocity>
              <acceleration>0 0 0 0 -0 0</acceleration>
              <wrench>0 0 0 0 -0 0</wrench>
            </link>
          </model>
          <light name='sun'>
            <pose>0 0 10 0 -0 0</pose>
          </light>
        </state>
        <gui fullscreen='0'>
          <camera name='user_camera'>
            <pose>5 -5 2 0 0.275643 2.35619</pose>
            <view_controller>orbit</view_controller>
            <projection_type>perspective</projection_type>
          </camera>
        </gui>
        """
    return str_head

def CreateRunwayWorld():
    str_head = """
    <sdf version='1.7'>
  <world name='default'>
    <light name='sun' type='directional'>
      <cast_shadows>1</cast_shadows>
      <pose>0 0 10 0 -0 0</pose>
      <diffuse>0.8 0.8 0.8 1</diffuse>
      <specular>0.2 0.2 0.2 1</specular>
      <attenuation>
        <range>1000</range>
        <constant>0.9</constant>
        <linear>0.01</linear>
        <quadratic>0.001</quadratic>
      </attenuation>
      <direction>-0.5 0.1 -0.9</direction>
      <spot>
        <inner_angle>0</inner_angle>
        <outer_angle>0</outer_angle>
        <falloff>0</falloff>
      </spot>
    </light>
    <model name="ground_plane">
      <static>true</static>
      <link name="link">
        <collision name="collision">
          <geometry>
            <plane>
              <normal>0 0 1</normal>
              <size>5000 5000</size>
            </plane>
          </geometry>
          <surface>
            <friction>
              <ode>
                <mu>1</mu>
                <mu2>1</mu2>
              </ode>
            </friction>
          </surface>
        </collision>
        <visual name="runway">
          <pose>000 0 0.005 0 0 -3.1415</pose>
          <cast_shadows>false</cast_shadows>
          <geometry>
            <plane>
              <normal>0 0 1</normal>
              <size>180 10</size> <!-- 1829 45 -->
            </plane>
          </geometry>
          <material>
            <script>
              <uri>file://media/materials/scripts/gazebo.material</uri>
              <name>Gazebo/Runway</name>
            </script>
          </material>
        </visual>

        <visual name="grass">
          <pose>0 0 -0.1 0 0 0</pose>
          <cast_shadows>false</cast_shadows>
          <geometry>
            <plane>
              <normal>0 0 1</normal>
              <size>5000 5000</size>
            </plane>
          </geometry>
          <material>
            <script>
              <uri>file://media/materials/scripts/gazebo.material</uri>
              <name>Gazebo/Grass</name>
            </script>
          </material>
        </visual>

      </link>
    </model>

    <!--
    <model name='ground_plane'>
      <static>1</static>
      <link name='link'>
        <collision name='collision'>
          <geometry>
            <plane>
              <normal>0 0 1</normal>
              <size>100 100</size>
            </plane>
          </geometry>
          <surface>
            <friction>
              <ode>
                <mu>100</mu>
                <mu2>50</mu2>
              </ode>
              <torsional>
                <ode/>
              </torsional>
            </friction>
            <contact>
              <ode/>
            </contact>
            <bounce/>
          </surface>
          <max_contacts>10</max_contacts>
        </collision>
        <visual name='visual'>
          <cast_shadows>0</cast_shadows>
          <geometry>
            <plane>
              <normal>0 0 1</normal>
              <size>100 100</size>
            </plane>
          </geometry>
          <material>
            <script>
              <uri>file://media/materials/scripts/gazebo.material</uri>
              <name>Gazebo/Grey</name>
            </script>
          </material>
        </visual>
        <self_collide>0</self_collide>
        <enable_wind>0</enable_wind>
        <kinematic>0</kinematic>
      </link>
    </model>
    -->
    <gravity>0 0 -9.8</gravity>
    <magnetic_field>6e-06 2.3e-05 -4.2e-05</magnetic_field>
    <atmosphere type='adiabatic'/>
    <physics type='ode'>
      <max_step_size>0.001</max_step_size>
      <real_time_factor>1</real_time_factor>
      <real_time_update_rate>1000</real_time_update_rate>
    </physics>
    <scene>
      <ambient>0.4 0.4 0.4 1</ambient>
      <background>0.7 0.7 0.7 1</background>
      <shadows>1</shadows>
    </scene>
    <wind/>
    <spherical_coordinates> <!-- stillwater osu flight field-->
      <surface_model>EARTH_WGS84</surface_model>
      <latitude_deg>36.1624584</latitude_deg>
      <longitude_deg>-96.8361438</longitude_deg>
      <elevation>196</elevation>
      <heading_deg>0</heading_deg>
    </spherical_coordinates>
    """
    return str_head

def CreateQuadModel(fdm_addr, port_in, port_out, x_pos_init, y_pos_init, z_pos_init, instance):
    model_str = f"""
    <model name="iris_demo{instance}">
        <pose> {x_pos_init} {y_pos_init} {z_pos_init} 0 0 0 </pose>
        <include>
          <uri>model://iris_with_standoffs</uri>
        </include>

        <!-- plugins -->
        <plugin name="rotor_0_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_0</link_name>
        </plugin>
        <plugin name="rotor_0_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_0</link_name>
        </plugin>

        <plugin name="rotor_1_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_1</link_name>
        </plugin>
        <plugin name="rotor_1_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_1</link_name>
        </plugin>

        <plugin name="rotor_2_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_2</link_name>
        </plugin>
        <plugin name="rotor_2_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_2</link_name>
        </plugin>

        <plugin name="rotor_3_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_3</link_name>
        </plugin>
        <plugin name="rotor_3_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_3</link_name>
        </plugin>
        <plugin name="arducopter_plugin" filename="libArduPilotPlugin.so">
          <fdm_addr>{fdm_addr}</fdm_addr>
          <fdm_port_in>{port_in}</fdm_port_in>
          <fdm_port_out>{port_out}</fdm_port_out>
          <!--
              Require by APM :
              Only change model and gazebo from XYZ to XY-Z coordinates
          -->
          <modelXYZToAirplaneXForwardZDown>0 0 0 3.141593 0 0</modelXYZToAirplaneXForwardZDown>
          <gazeboXYZToNED>0 0 0 3.141593 0 0</gazeboXYZToNED>
          <imuName>iris_demo{instance+1}::iris::iris/imu_link::imu_sensor</imuName>
          <connectionTimeoutMaxCount>5</connectionTimeoutMaxCount>
          <control channel="0">
          <!--
              incoming control command [0, 1]
              so offset it by 0 to get [0, 1]
              and divide max target by 1.
              offset = 0
              multiplier = 838 max rpm / 1 = 838
            -->
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_0_joint</jointName>
            <multiplier>838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
          <control channel="1">
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_1_joint</jointName>
            <multiplier>838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
          <control channel="2">
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_2_joint</jointName>
            <multiplier>-838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
          <control channel="3">
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_3_joint</jointName>
            <multiplier>-838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
        </plugin>
      </model>"""

    return model_str

def CreateUAM_Model(fdm_addr, port_in, port_out, x_pos_init, y_pos_init, z_pos_init, instance):
    model_str = f"""
    <model name="iris_demo{instance}">
        <pose> {x_pos_init} {y_pos_init} {z_pos_init} 0 0 0 </pose>
        <include>
          <uri>model://iris_with_standoffs</uri>
        </include>

        <!-- plugins -->
        <plugin name="rotor_0_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_0</link_name>
        </plugin>
        <plugin name="rotor_0_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_0</link_name>
        </plugin>

        <plugin name="rotor_1_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_1</link_name>
        </plugin>
        <plugin name="rotor_1_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_1</link_name>
        </plugin>

        <plugin name="rotor_2_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_2</link_name>
        </plugin>
        <plugin name="rotor_2_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_2</link_name>
        </plugin>

        <plugin name="rotor_3_blade_1" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>0.084 0 0</cp>
          <forward>0 -1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_3</link_name>
        </plugin>
        <plugin name="rotor_3_blade_2" filename="libLiftDragPlugin.so">
          <a0>0.3</a0>
          <alpha_stall>1.4</alpha_stall>
          <cla>4.2500</cla>
          <cda>0.10</cda>
          <cma>0.00</cma>
          <cla_stall>-0.025</cla_stall>
          <cda_stall>0.0</cda_stall>
          <cma_stall>0.0</cma_stall>
          <area>0.002</area>
          <air_density>1.2041</air_density>
          <cp>-0.084 0 0</cp>
          <forward>0 1 0</forward>
          <upward>0 0 1</upward>
          <link_name>iris::rotor_3</link_name>
        </plugin>
        <plugin name="arducopter_plugin" filename="libArduPilotPlugin.so">
          <fdm_addr>{fdm_addr}</fdm_addr>
          <fdm_port_in>{port_in}</fdm_port_in>
          <fdm_port_out>{port_out}</fdm_port_out>
          <!--
              Require by APM :
              Only change model and gazebo from XYZ to XY-Z coordinates
          -->
          <modelXYZToAirplaneXForwardZDown>0 0 0 3.141593 0 0</modelXYZToAirplaneXForwardZDown>
          <gazeboXYZToNED>0 0 0 3.141593 0 0</gazeboXYZToNED>
          <imuName>iris_demo{instance+1}::iris::iris/imu_link::imu_sensor</imuName>
          <connectionTimeoutMaxCount>5</connectionTimeoutMaxCount>
          <control channel="0">
          <!--
              incoming control command [0, 1]
              so offset it by 0 to get [0, 1]
              and divide max target by 1.
              offset = 0
              multiplier = 838 max rpm / 1 = 838
            -->
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_0_joint</jointName>
            <multiplier>838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
          <control channel="1">
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_1_joint</jointName>
            <multiplier>838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
          <control channel="2">
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_2_joint</jointName>
            <multiplier>-838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
          <control channel="3">
            <type>VELOCITY</type>
            <offset>0</offset>
            <p_gain>0.20</p_gain>
            <i_gain>0</i_gain>
            <d_gain>0</d_gain>
            <i_max>0</i_max>
            <i_min>0</i_min>
            <cmd_max>2.5</cmd_max>
            <cmd_min>-2.5</cmd_min>
            <jointName>iris::rotor_3_joint</jointName>
            <multiplier>-838</multiplier>
            <controlVelocitySlowdownSim>1</controlVelocitySlowdownSim>
          </control>
        </plugin>
      </model>"""
    return model_str


def CreateFWModel(fdm_addr, port_in, port_out, x_pos_init, y_pos_init, z_pos_init, instance):
    model_str = f"""
    <model name="zephyr_delta_wing_demo{instance}">
    <pose>{x_pos_init} {y_pos_init} {z_pos_init} 0 0 0</pose>
    <include>
      <uri>model://zephyr_delta_wing</uri>
      <!--
        IMPORTANT (for now):
        zephyr_delta_wing model is -y forward, x-left and z-up
        ardupilot plugin requires x-forward, y-left, z-up
        for things to work properly, so it's important to set pose below
      -->
      <pose>0 0 0.2 0 0 0</pose>
    </include>
    <plugin name="ardupilot_plugin" filename="libArduPilotPlugin.so">
      <fdm_addr>{fdm_addr}</fdm_addr>
      <fdm_port_in>{port_in}</fdm_port_in>
      <fdm_port_out>{port_out}</fdm_port_out>
      <modelXYZToAirplaneXForwardZDown>0 0 0 3.141593 0 -1.57079</modelXYZToAirplaneXForwardZDown>
      <gazeboXYZToNED>0 0 0 3.141593 0 -1.57079</gazeboXYZToNED>
      <imuName>zephyr_delta_wing_demo{instance}::zephyr_delta_wing::zephyr/imu_link::imu_sensor</imuName>
      <connectionTimeoutMaxCount>5</connectionTimeoutMaxCount>
      <control channel="2">
        <!--
          incoming control command [0, 1]
          so offset it by 0 to get [0, 1]
          and divide max target by 1.
            offset = 0
            multiplier = 838 max rpm / 1 = 838
        -->
        <multiplier>838</multiplier>
        <offset>0.2</offset>
        <type>VELOCITY</type>
        <p_gain>0.5</p_gain>
        <i_gain>0</i_gain>
        <d_gain>0</d_gain>
        <i_max>0</i_max>
        <i_min>0</i_min>
        <cmd_max>2.0</cmd_max>
        <cmd_min>0.0</cmd_min>
        <jointName>zephyr_delta_wing::propeller_joint</jointName>
      </control>
      <control channel="0">
        <multiplier>-1</multiplier>
        <offset>-0.5</offset>
        <type>POSITION</type>
        <p_gain>10.0</p_gain>
        <i_gain>0</i_gain>
        <d_gain>0</d_gain>
        <i_max>0</i_max>
        <i_min>0</i_min>
        <cmd_max>2.0</cmd_max>
        <cmd_min>-2.0</cmd_min>
        <jointName>zephyr_delta_wing::flap_left_joint</jointName>
      </control>
      <control channel="1">
        <multiplier>-1</multiplier>
        <offset>-0.5</offset>
        <type>POSITION</type>
        <p_gain>10.0</p_gain>
        <i_gain>0</i_gain>
        <d_gain>0</d_gain>
        <i_max>0</i_max>
        <i_min>0</i_min>
        <cmd_max>2.0</cmd_max>
        <cmd_min>-2.0</cmd_min>
        <jointName>zephyr_delta_wing::flap_right_joint</jointName>
      </control>
    </plugin>
  </model>
"""

    return model_str

def GetFooter():
    str_end = """
      </world>
    </sdf>
    """

    return str_end
