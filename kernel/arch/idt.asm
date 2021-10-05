[bits 32]

; IDT handlers 'n stuff

global idt_table
global idt_list
extern idt_call

idt_table:
.entry_0:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_1:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_2:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_3:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_4:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_5:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_6:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_7:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_8:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_9:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_10:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_11:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_12:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_13:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_14:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_15:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_16:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_17:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_18:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_19:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_20:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_21:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_22:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_23:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_24:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_25:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_26:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_27:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_28:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_29:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_30:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_31:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8F
  dw 0x0000
.entry_32:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_33:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_34:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_35:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_36:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_37:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_38:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_39:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_40:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_41:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_42:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_43:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_44:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_45:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_46:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_47:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_48:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_49:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_50:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_51:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_52:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_53:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_54:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_55:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_56:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_57:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_58:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_59:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_60:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_61:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_62:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_63:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_64:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_65:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_66:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_67:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_68:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_69:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_70:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_71:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_72:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_73:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_74:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_75:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_76:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_77:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_78:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_79:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_80:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_81:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_82:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_83:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_84:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_85:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_86:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_87:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_88:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_89:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_90:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_91:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_92:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_93:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_94:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_95:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_96:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_97:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_98:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_99:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_100:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_101:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_102:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_103:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_104:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_105:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_106:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_107:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_108:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_109:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_110:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_111:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_112:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_113:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_114:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_115:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_116:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_117:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_118:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_119:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_120:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_121:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_122:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_123:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_124:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_125:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_126:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_127:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_128:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_129:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_130:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_131:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_132:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_133:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_134:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_135:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_136:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_137:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_138:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_139:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_140:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_141:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_142:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_143:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_144:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_145:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_146:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_147:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_148:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_149:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_150:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_151:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_152:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_153:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_154:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_155:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_156:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_157:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_158:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_159:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_160:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_161:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_162:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_163:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_164:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_165:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_166:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_167:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_168:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_169:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_170:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_171:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_172:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_173:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_174:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_175:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_176:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_177:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_178:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_179:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_180:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_181:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_182:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_183:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_184:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_185:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_186:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_187:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_188:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_189:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_190:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_191:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_192:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_193:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_194:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_195:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_196:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_197:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_198:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_199:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_200:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_201:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_202:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_203:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_204:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_205:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_206:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_207:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_208:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_209:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_210:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_211:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_212:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_213:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_214:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_215:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_216:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_217:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_218:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_219:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_220:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_221:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_222:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_223:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_224:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_225:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_226:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_227:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_228:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_229:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_230:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_231:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_232:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_233:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_234:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_235:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_236:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_237:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_238:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_239:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_240:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_241:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_242:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_243:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_244:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_245:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_246:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_247:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_248:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_249:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_250:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_251:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_252:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_253:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_254:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000
.entry_255:
  dw 0x0000
  dw 0x0008
  db 0x00
  db 0x8E
  dw 0x0000

idt_list:
  dd idt_hand_0
  dd idt_hand_1
  dd idt_hand_2
  dd idt_hand_3
  dd idt_hand_4
  dd idt_hand_5
  dd idt_hand_6
  dd idt_hand_7
  dd idt_hand_8
  dd idt_hand_9
  dd idt_hand_10
  dd idt_hand_11
  dd idt_hand_12
  dd idt_hand_13
  dd idt_hand_14
  dd idt_hand_15
  dd idt_hand_16
  dd idt_hand_17
  dd idt_hand_18
  dd idt_hand_19
  dd idt_hand_20
  dd idt_hand_21
  dd idt_hand_22
  dd idt_hand_23
  dd idt_hand_24
  dd idt_hand_25
  dd idt_hand_26
  dd idt_hand_27
  dd idt_hand_28
  dd idt_hand_29
  dd idt_hand_30
  dd idt_hand_31
  dd idt_hand_32
  dd idt_hand_33
  dd idt_hand_34
  dd idt_hand_35
  dd idt_hand_36
  dd idt_hand_37
  dd idt_hand_38
  dd idt_hand_39
  dd idt_hand_40
  dd idt_hand_41
  dd idt_hand_42
  dd idt_hand_43
  dd idt_hand_44
  dd idt_hand_45
  dd idt_hand_46
  dd idt_hand_47
  dd idt_hand_48
  dd idt_hand_49
  dd idt_hand_50
  dd idt_hand_51
  dd idt_hand_52
  dd idt_hand_53
  dd idt_hand_54
  dd idt_hand_55
  dd idt_hand_56
  dd idt_hand_57
  dd idt_hand_58
  dd idt_hand_59
  dd idt_hand_60
  dd idt_hand_61
  dd idt_hand_62
  dd idt_hand_63
  dd idt_hand_64
  dd idt_hand_65
  dd idt_hand_66
  dd idt_hand_67
  dd idt_hand_68
  dd idt_hand_69
  dd idt_hand_70
  dd idt_hand_71
  dd idt_hand_72
  dd idt_hand_73
  dd idt_hand_74
  dd idt_hand_75
  dd idt_hand_76
  dd idt_hand_77
  dd idt_hand_78
  dd idt_hand_79
  dd idt_hand_80
  dd idt_hand_81
  dd idt_hand_82
  dd idt_hand_83
  dd idt_hand_84
  dd idt_hand_85
  dd idt_hand_86
  dd idt_hand_87
  dd idt_hand_88
  dd idt_hand_89
  dd idt_hand_90
  dd idt_hand_91
  dd idt_hand_92
  dd idt_hand_93
  dd idt_hand_94
  dd idt_hand_95
  dd idt_hand_96
  dd idt_hand_97
  dd idt_hand_98
  dd idt_hand_99
  dd idt_hand_100
  dd idt_hand_101
  dd idt_hand_102
  dd idt_hand_103
  dd idt_hand_104
  dd idt_hand_105
  dd idt_hand_106
  dd idt_hand_107
  dd idt_hand_108
  dd idt_hand_109
  dd idt_hand_110
  dd idt_hand_111
  dd idt_hand_112
  dd idt_hand_113
  dd idt_hand_114
  dd idt_hand_115
  dd idt_hand_116
  dd idt_hand_117
  dd idt_hand_118
  dd idt_hand_119
  dd idt_hand_120
  dd idt_hand_121
  dd idt_hand_122
  dd idt_hand_123
  dd idt_hand_124
  dd idt_hand_125
  dd idt_hand_126
  dd idt_hand_127
  dd idt_hand_128
  dd idt_hand_129
  dd idt_hand_130
  dd idt_hand_131
  dd idt_hand_132
  dd idt_hand_133
  dd idt_hand_134
  dd idt_hand_135
  dd idt_hand_136
  dd idt_hand_137
  dd idt_hand_138
  dd idt_hand_139
  dd idt_hand_140
  dd idt_hand_141
  dd idt_hand_142
  dd idt_hand_143
  dd idt_hand_144
  dd idt_hand_145
  dd idt_hand_146
  dd idt_hand_147
  dd idt_hand_148
  dd idt_hand_149
  dd idt_hand_150
  dd idt_hand_151
  dd idt_hand_152
  dd idt_hand_153
  dd idt_hand_154
  dd idt_hand_155
  dd idt_hand_156
  dd idt_hand_157
  dd idt_hand_158
  dd idt_hand_159
  dd idt_hand_160
  dd idt_hand_161
  dd idt_hand_162
  dd idt_hand_163
  dd idt_hand_164
  dd idt_hand_165
  dd idt_hand_166
  dd idt_hand_167
  dd idt_hand_168
  dd idt_hand_169
  dd idt_hand_170
  dd idt_hand_171
  dd idt_hand_172
  dd idt_hand_173
  dd idt_hand_174
  dd idt_hand_175
  dd idt_hand_176
  dd idt_hand_177
  dd idt_hand_178
  dd idt_hand_179
  dd idt_hand_180
  dd idt_hand_181
  dd idt_hand_182
  dd idt_hand_183
  dd idt_hand_184
  dd idt_hand_185
  dd idt_hand_186
  dd idt_hand_187
  dd idt_hand_188
  dd idt_hand_189
  dd idt_hand_190
  dd idt_hand_191
  dd idt_hand_192
  dd idt_hand_193
  dd idt_hand_194
  dd idt_hand_195
  dd idt_hand_196
  dd idt_hand_197
  dd idt_hand_198
  dd idt_hand_199
  dd idt_hand_200
  dd idt_hand_201
  dd idt_hand_202
  dd idt_hand_203
  dd idt_hand_204
  dd idt_hand_205
  dd idt_hand_206
  dd idt_hand_207
  dd idt_hand_208
  dd idt_hand_209
  dd idt_hand_210
  dd idt_hand_211
  dd idt_hand_212
  dd idt_hand_213
  dd idt_hand_214
  dd idt_hand_215
  dd idt_hand_216
  dd idt_hand_217
  dd idt_hand_218
  dd idt_hand_219
  dd idt_hand_220
  dd idt_hand_221
  dd idt_hand_222
  dd idt_hand_223
  dd idt_hand_224
  dd idt_hand_225
  dd idt_hand_226
  dd idt_hand_227
  dd idt_hand_228
  dd idt_hand_229
  dd idt_hand_230
  dd idt_hand_231
  dd idt_hand_232
  dd idt_hand_233
  dd idt_hand_234
  dd idt_hand_235
  dd idt_hand_236
  dd idt_hand_237
  dd idt_hand_238
  dd idt_hand_239
  dd idt_hand_240
  dd idt_hand_241
  dd idt_hand_242
  dd idt_hand_243
  dd idt_hand_244
  dd idt_hand_245
  dd idt_hand_246
  dd idt_hand_247
  dd idt_hand_248
  dd idt_hand_249
  dd idt_hand_250
  dd idt_hand_251
  dd idt_hand_252
  dd idt_hand_253
  dd idt_hand_254
  dd idt_hand_255

idt_hand_0:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 0
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_1:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 1
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_2:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 2
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_3:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 3
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_4:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 4
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_5:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 5
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_6:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 6
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_7:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 7
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_8:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 8
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_9:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 9
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_10:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 10
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_11:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 11
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_12:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 12
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_13:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 13
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_14:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 14
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_15:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 15
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_16:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 16
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_17:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 17
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_18:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 18
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_19:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 19
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_20:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 20
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_21:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 21
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_22:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 22
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_23:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 23
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_24:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 24
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_25:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 25
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_26:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 26
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_27:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 27
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_28:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 28
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_29:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 29
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_30:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 30
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_31:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 31
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_32:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 32
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_33:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 33
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_34:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 34
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_35:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 35
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_36:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 36
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_37:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 37
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_38:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 38
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_39:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 39
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  iret

idt_hand_40:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 40
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_41:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 41
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_42:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 42
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_43:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 43
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_44:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 44
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_45:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 45
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_46:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 46
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_47:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 47
  push eax
  call idt_call
  add esp, (2 * 4)
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  mov dl, 0xA0
  out dx, al
  popad
  iret

idt_hand_48:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 48
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_49:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 49
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_50:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 50
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_51:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 51
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_52:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 52
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_53:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 53
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_54:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 54
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_55:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 55
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_56:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 56
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_57:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 57
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_58:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 58
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_59:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 59
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_60:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 60
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_61:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 61
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_62:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 62
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_63:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 63
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_64:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 64
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_65:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 65
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_66:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 66
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_67:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 67
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_68:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 68
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_69:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 69
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_70:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 70
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_71:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 71
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_72:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 72
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_73:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 73
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_74:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 74
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_75:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 75
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_76:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 76
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_77:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 77
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_78:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 78
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_79:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 79
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_80:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 80
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_81:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 81
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_82:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 82
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_83:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 83
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_84:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 84
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_85:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 85
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_86:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 86
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_87:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 87
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_88:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 88
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_89:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 89
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_90:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 90
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_91:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 91
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_92:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 92
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_93:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 93
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_94:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 94
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_95:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 95
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_96:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 96
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_97:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 97
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_98:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 98
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_99:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 99
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_100:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 100
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_101:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 101
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_102:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 102
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_103:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 103
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_104:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 104
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_105:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 105
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_106:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 106
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_107:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 107
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_108:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 108
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_109:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 109
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_110:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 110
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_111:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 111
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_112:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 112
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_113:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 113
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_114:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 114
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_115:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 115
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_116:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 116
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_117:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 117
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_118:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 118
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_119:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 119
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_120:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 120
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_121:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 121
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_122:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 122
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_123:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 123
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_124:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 124
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_125:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 125
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_126:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 126
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_127:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 127
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_128:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 128
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_129:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 129
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_130:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 130
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_131:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 131
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_132:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 132
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_133:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 133
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_134:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 134
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_135:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 135
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_136:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 136
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_137:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 137
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_138:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 138
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_139:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 139
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_140:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 140
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_141:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 141
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_142:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 142
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_143:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 143
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_144:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 144
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_145:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 145
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_146:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 146
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_147:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 147
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_148:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 148
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_149:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 149
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_150:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 150
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_151:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 151
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_152:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 152
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_153:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 153
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_154:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 154
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_155:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 155
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_156:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 156
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_157:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 157
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_158:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 158
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_159:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 159
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_160:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 160
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_161:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 161
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_162:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 162
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_163:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 163
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_164:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 164
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_165:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 165
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_166:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 166
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_167:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 167
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_168:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 168
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_169:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 169
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_170:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 170
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_171:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 171
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_172:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 172
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_173:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 173
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_174:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 174
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_175:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 175
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_176:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 176
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_177:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 177
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_178:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 178
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_179:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 179
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_180:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 180
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_181:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 181
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_182:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 182
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_183:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 183
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_184:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 184
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_185:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 185
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_186:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 186
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_187:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 187
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_188:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 188
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_189:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 189
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_190:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 190
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_191:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 191
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_192:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 192
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_193:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 193
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_194:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 194
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_195:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 195
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_196:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 196
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_197:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 197
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_198:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 198
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_199:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 199
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_200:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 200
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_201:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 201
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_202:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 202
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_203:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 203
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_204:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 204
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_205:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 205
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_206:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 206
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_207:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 207
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_208:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 208
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_209:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 209
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_210:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 210
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_211:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 211
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_212:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 212
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_213:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 213
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_214:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 214
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_215:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 215
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_216:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 216
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_217:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 217
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_218:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 218
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_219:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 219
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_220:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 220
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_221:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 221
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_222:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 222
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_223:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 223
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_224:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 224
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_225:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 225
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_226:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 226
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_227:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 227
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_228:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 228
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_229:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 229
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_230:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 230
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_231:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 231
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_232:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 232
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_233:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 233
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_234:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 234
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_235:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 235
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_236:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 236
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_237:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 237
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_238:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 238
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_239:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 239
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_240:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 240
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_241:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 241
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_242:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 242
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_243:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 243
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_244:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 244
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_245:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 245
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_246:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 246
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_247:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 247
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_248:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 248
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_249:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 249
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_250:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 250
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_251:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 251
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_252:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 252
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_253:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 253
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_254:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 254
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret

idt_hand_255:
  pushad
  mov eax, esp
  ; add eax, (9 * 4)
  push dword 255
  push eax
  call idt_call
  add esp, (2 * 4)
  popad
  iret
