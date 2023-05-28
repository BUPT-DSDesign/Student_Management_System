import { defineStore } from 'pinia'
import { EventTable, AddEvent, DeleteEvent } from '@/api/event'
import { ref } from 'vue'

export const useEventStore = defineStore('event', () => {
    const eventList = ref([])
    const GetEventTable = async () => {
        try {
            const res = await EventTable()

            if (res.data.status_code == 0) {
                eventList.value = res.data.activities
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }
    //添加活动
    const AddEventInfo = async (data) => {
        try {
            const res = await AddEvent(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
            else {
                this.$message({
                    showClose: true,
                    center: true,
                    message: res.data.status_msg,
                    type: 'error'
                });
            }
            return false
        } catch (err) {
            return false
        }
    }

    //删除活动
    const DeleteEventInfo = async (data) => {
        try {
            const res = await DeleteEvent(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }

    return {
        eventList,
        GetEventTable,
        AddEventInfo,
        DeleteEventInfo
    }
})
