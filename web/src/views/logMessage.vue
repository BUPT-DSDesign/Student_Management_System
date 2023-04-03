<template>
    <div class="container">
        <h3 style="color:black">日志信息记录表</h3>
        <el-table :data="logs" style="width: 100%" height="500" stripe>
            <el-table-column fixed label="用户" width="60">
                <template>
                    <img :src="avatarUrl" class="user-avatar">
                </template>
            </el-table-column>
            <el-table-column fixed label="时间" align="center">
                <template slot-scope="scope">
                    <i class="el-icon-time"></i>
                    <span style="margin-left: 10px">{{ scope.row.create_time }}</span>
                </template>
            </el-table-column>
            <el-table-column fixed label="内容" align="center">
                <template slot-scope="scope">
                    <i class="el-icon-document"></i>
                    <span style="margin-left: 10px">{{ scope.row.content }}</span>
                </template>
            </el-table-column>
            <el-table-column fixed label="操作" align="center">
                <template slot-scope="scope">
                    <el-button type="text" style="color: red;" @click="deleteLog(scope.row.log_id)">
                        <i class="el-icon-delete"></i>
                        <span style="margin-left: 5px">删除</span>
                    </el-button>
                   
                </template>
            </el-table-column>
        </el-table>    
        <el-dialog
            title="提示"
            :visible.sync="dialogVisible"
            width="30%"
            center>
            <span style="color: red">确定要删除这条日志吗？</span>
            <span slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible = false">取 消</el-button>
                <el-button type="primary" @click="deleteLogTruely">确 定</el-button>
            </span>
        </el-dialog>
        <el-button @click="exportToFile" type="success" style="float:right">导出日志文件</el-button>
    </div>
</template>

<script>
import { LogStore } from '@/store/log';
import { UserStore } from '@/store/user';
import Blob from 'blob';

export default {
    data() {
        return {
            logs: '',
            dialogVisible: false,
            tmpLogId: 0,
            avatarUrl: ''
        }
    },
    mounted() {
        this.logs = LogStore.logs
        this.avatarUrl = UserStore.userInfo.avatar_url 
    },
    methods: {
        deleteLog(logId) {
            this.tmpLogId = logId
            this.dialogVisible = true
        },
        deleteLogTruely() {
            this.dialogVisible = false
            console.log(this.tmpLogId)
        },
        exportToFile() {
            let data = ''
            for (let i = 0; i < this.logs.length; i++) {
                const time = this.logs[i].create_time
                const content = this.logs[i].content
                data += time + '    ' + content + '\n' 
            }
            const blob = new Blob([data], { type: "text/plain" })
            const downloadUrl = window.URL.createObjectURL(blob) // 文件下载路径
            const link = document.createElement("a") 
            link.href = downloadUrl
            link.download = "filename.txt"
            link.click()
            window.URL.revokeObjectURL(downloadUrl)
        }
    }
}
</script>

<style>
.user-avatar {
    width: 30px;
    height: 30px;
    border-radius: 50%;
}
</style>