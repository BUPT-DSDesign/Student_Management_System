<template>
    <div class="container">
        <h3 style="color:black">
            <i class="el-icon-notebook-1"></i>日志信息记录表
        </h3>
        <el-table :data="logs" style="width: 100%" :max-height="maxHeight" stripe :show-summary="false" @selection-change="handleSelectionChange">
            <el-table-column type="selection" width="55" v-if="batchSelect"></el-table-column>
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
        <el-button @click="batchDelete" type="danger" style="float:left" v-if="!afterBatchDelete">批量删除日志</el-button>
        <el-button @click="cancelBatchDelete" style="float:left" v-if="afterBatchDelete">取消</el-button>
        <el-button @click="confirmBatchDelete" style="float:left" type="primary" v-if="afterBatchDelete">确定</el-button>
        <el-button @click="exportToFile" type="success" style="float:right;margin-right: 17px;">导出日志文件</el-button>  
        <el-dialog
            title="提示"
            :visible.sync="dialogVisible"
            width="30%"
            center>
            <span style="color: red">确定要删除这些（条）日志吗？</span>
            <span slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible = false">取 消</el-button>
                <el-button type="primary" @click="deleteLogTruely">确 定</el-button>
            </span>
        </el-dialog>
        
    </div>
</template>

<script>
import { LogStore } from '@/store/log';
import { UserStore } from '@/store/user';
import { Loading } from 'element-ui';
// import Blob from 'blob';

export default {
    data() {
        return {
            logs: LogStore.logs,
            dialogVisible: false,
            tmpLogIds: [],
            avatarUrl: UserStore.userInfo.avatar_url,
            loadingInstance: '',
            maxHeight: document.body.clientHeight - 153,
            batchSelect: false,
            afterBatchDelete: false
        }
    },
    methods: {
        deleteLog(logId) {
            this.tmpLogIds = []
            this.tmpLogIds.push(logId)
            this.dialogVisible = true
        },
        deleteLogTruely() {
            this.dialogVisible = false
            if (this.tmpLogIds.length == 0) {
                this.$message({
                    showClose: true,
                    center: true,
                    message: '您未选择要删除的日志',
                    type: 'warning'
                });
                return
            }
            const deleteLog = async () => {
                let jso = {}
                for (let i = 0; i < this.tmpLogIds.length; i++) {
                    jso[i] = this.tmpLogIds[i]
                }
                const logIdString = JSON.stringify(jso)
                // console.log(logIdString)
                const fg = await LogStore.DeleteLog(logIdString)
                if (fg) {
                    this.batchSelect = false
                    this.afterBatchDelete = false
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '删除日志成功',
                        type: 'success'
                    });
                    this.logs = LogStore.logs     
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '删除日志失败',
                        type: 'error'
                    });
                }
            }
            deleteLog()
        },
        batchDelete() {
            this.batchSelect = true
            this.afterBatchDelete = true
        },
        handleSelectionChange(val) {
            this.tmpLogIds = []
            val.forEach((data) => {
                this.tmpLogIds.push(data.log_id)
            })
            console.log(this.tmpLogIds)
        },
        cancelBatchDelete() {
            this.batchSelect = false
            this.afterBatchDelete = false
        },
        confirmBatchDelete() {
            
            this.dialogVisible = true
        },
        exportToFile() {
            let data = ''
            for (let i = 0; i < this.logs.length; i++) {
                const time = this.logs[i].crneate_time
                const content = this.logs[i].content
                data += time + '    ' + content + '\n' 
            }
            const blob = new Blob([data], { type: "text/plain" })
            const downloadUrl = window.URL.createObjectURL(blob) // 文件下载路径
            const link = document.createElement("a") 
            link.href = downloadUrl
            link.download = 'log.txt'
            this.loadingInstance = Loading.service({
                lock: true,
                text: '导出文件中，请稍候.....',
                background: 'rgba(0, 0, 0, 0.7)'
            })
            link.click()
            window.URL.revokeObjectURL(downloadUrl)
            setTimeout(() => {
                console.log('error')
                this.loadingInstance.close()
            }, 2500);
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