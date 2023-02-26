<template>
    <div class="container">
        <div class="schedule-container">
            <h2>日程安排表</h2>
            <div>
                <el-table :data="tableData" :default-sort="{ prop: 'date', order: 'descending' }">
                    <el-table-column prop="date" label="日期" sortable width="180">
                    </el-table-column>
                    <el-table-column prop="time" label="时间" sortable width="180">
                    </el-table-column>
                    <el-table-column prop="name" label="活动名称" :formatter="formatter">
                    </el-table-column>
                </el-table>
            </div>
        </div>
        <div class="button-container">
            <!-- 添加活动 -->
            <el-button class="OperationButton" type="primary" icon="el-icon-circle-plus-outline"
                @click="dialogFormVisible = true">添加活动</el-button>
            <el-dialog title="添加活动" :visible.sync="dialogFormVisible">
                <el-form :model="form">
                    <el-form-item label="活动名称" :label-width="formLabelWidth">
                        <el-input v-model="form.name" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动地点" :label-width="formLabelWidth">
                        <el-input v-model="form.address" autocomplete="off"></el-input>
                    </el-form-item>
                    <el-form-item label="活动起止时间" :label-width="formLabelWidth">
                        <el-time-picker is-range v-model="value1" range-separator="至" start-placeholder="开始时间"
                            end-placeholder="结束时间" placeholder="选择时间范围">
                        </el-time-picker>
                    </el-form-item>
                    <el-form-item label="活动类型" :label-width="formLabelWidth">
                        <el-radio v-model="radio" label="1">个人活动</el-radio>
                        <el-radio v-model="radio" label="2">集体活动</el-radio>
                    </el-form-item>
                </el-form>
                <div slot="footer" class="dialog-footer">
                    <el-button @click="dialogFormVisible = false">取 消</el-button>
                    <el-button type="primary" @click="dialogFormVisible = false">确 定</el-button>
                </div>
            </el-dialog>
            <el-button class="OperationButton" type="primary" icon="el-icon-search">活动查询</el-button>
            <el-button class="OperationButton" type="primary" icon="el-icon-alarm-clock">闹钟提醒</el-button>
        </div>
    </div>
</template>

<script>
export default {
    data() {
        return {
            dialogFormVisible: false,
            form: {
                name: '',
                address: '',
            },
            formLabelWidth: '120px',
            value1: [new Date(2016, 9, 10, 8, 40), new Date(2016, 9, 10, 9, 40)],
            tableData: [{
                date: '2023-02-02',
                time: '15:00',
                name: '班会'
            }, {
                date: '2023-02-04',
                time: '14:00',
                name: '取快递'
            }, {
                date: '2023-02-01',
                time: '14:00',
                name: '计算机网络考试'
            }, {
                date: '2023-02-03',
                time: '14:00',
                name: '出门游玩'
            }]
        }
    },
    methods: {
        formatter(row, column) {
            return row.name;
        }
    }
}
</script>
<style>
h2 {
    margin: 15px;
    margin-left: 200px;
    font-size: 20px;
}

.schedule-container {
    margin-left: 300px;
}

.button-container {
    margin-left: 300px;
}

.OperationButton {
    margin: 20px
}
</style>